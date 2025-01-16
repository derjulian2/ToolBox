////////////////////////////////////////////////////////////
#include "XMLParser.hpp"
////////////////////////////////////////////////////////////
XMLAttribute::XMLAttribute(std::string _name, std::string _val) : name(_name), value(_val)
{

}
std::string XMLAttribute::getName() const
{
	return name;
}
void XMLAttribute::setName(const std::string& str)
{
	name = str;
}
std::string XMLAttribute::getValue() const
{
	return value;
}
void XMLAttribute::setValue(const std::string& str)
{
	value = str;
}
std::string XMLAttribute::toXML() const
{
	return name + "=\"" + value + "\"";
}
std::ostream& operator<<(std::ostream& os, const XMLAttribute& atr)
{
	os << atr.name << "='" << atr.value << "'";
	return os;
}
////////////////////////////////////////////////////////////
XMLTag::XMLTag(const std::string& _name) : name(_name)
{

}
XMLTag::XMLTag(const std::string& _name, const std::string& _val) : name(_name), value(_val)
{

}
std::string XMLTag::getName() const
{
	return name;
}
void XMLTag::setName(const std::string& str)
{
	name = str;
}
std::string XMLTag::getValue() const
{
	return value;
}
void XMLTag::setValue(const std::string& str)
{
	value = str;
}
std::list<XMLTag>& XMLTag::getSubTags()
{
	return subtags;
}
std::list<XMLAttribute>& XMLTag::getAttributes()
{
	return attributes;
}
void XMLTag::makeProcInstruction()
{
	_PROC_ = true;
	value.clear();
	name.insert(0, "?");
}
void XMLTag::removeProcInstruction()
{
	_PROC_ = false;
	name = name.substr(name.find_first_of('?') + 1);
}
XMLTag* XMLTag::getParentTag()
{
	return parent;
}
void XMLTag::setParentTag(XMLTag& tag)
{
	parent = &tag;
}
XMLTag& XMLTag::AddTag(const std::string& _name)
{
	return AddTag(XMLTag(_name));
}
XMLTag& XMLTag::AddTag(const std::string& _name, const std::string& _val)
{
	return AddTag(XMLTag(_name, _val));
}
XMLTag& XMLTag::AddTag(const XMLTag& subtag)
{
	subtags.emplace_back(subtag);
	subtags.back().depth = depth + 1;
	subtags.back().parent = this;
	value.clear();
	return subtags.back();
}
XMLTag &XMLTag::FindTag(const std::string &_name)
{
	for (XMLTag& tag : subtags)
	{
		if (tag.name == _name)
			return tag;
		if (!tag.getSubTags().empty())
			return tag.FindTag(_name);
	}
	return *this;
}
XMLAttribute& XMLTag::AddAttribute(const std::string& _name, const std::string& _val)
{
	return AddAttribute(XMLAttribute(_name, _val));
}
XMLAttribute& XMLTag::AddAttribute(const XMLAttribute& atr)
{
	attributes.emplace_back(atr);
	return attributes.back();
}
std::list<XMLAttribute>& XMLTag::AddAttributes(const std::vector<std::string>& _val)
{
	if (_val.size() % 2)
		throw std::runtime_error("invalid attribute input: XML-attributes always come in name-value pairs");
	std::string temp;
	for (uint64_t iter = 0; iter < _val.size(); iter++)
	{
		if (!(iter % 2))
			temp = _val[iter];
		else
			AddAttribute(temp, _val[iter]);
	}
	return attributes;
}
std::list<XMLAttribute>& XMLTag::AddAttributes(const std::vector<XMLAttribute>& _val)
{
	for (const XMLAttribute& attr : _val)
		AddAttribute(attr);
	return attributes;
}
std::string XMLTag::toXML() const
{
	std::stringstream res;
	// print depth indentation
	for (uint64_t i = 0; i < depth; i++)
		res << ' ';
	// print opening tag and attributes
	res << '<' << name;
	for (const XMLAttribute& atr : attributes)
		res << ' ' << atr.toXML();
	if (_PROC_)
		res << '?';
	res << '>';
	// print subtags, if any
	if (subtags.size() != 0)
	{
		for (const XMLTag& tag : subtags)
			res << '\n' << tag.toXML();
	}
	else
		// print value if no subtags
		res << value;
	if (subtags.size() != 0)
	{
		// adjust indentation line breaks for subtags
		res << '\n';
		for (uint64_t i = 0; i < depth; i++)
			res << ' ';
	}
	// closing tag
	if (_PROC_ && parent == nullptr)
		res << '\n';
	if (!_PROC_)
		res << '<' << '/' << name << '>';
	return res.str();
}
std::ostream& operator<<(std::ostream& os, const XMLTag& tag)
{
	if (tag.attributes.size() != 0)
	{
		os << tag.name << " (";
		for (const XMLAttribute& atr : tag.attributes)
		{
			os << atr.getName() << '=' << atr.getValue();
			if (&atr != &tag.attributes.back())
				os << ',' << ' ';
		}
		os << "): " << tag.value << "\n";
	}
	else
		os << tag.name << ": " << tag.value << "\n";
	for (XMLTag subtag : tag.subtags)
	{
		for (uint16_t i = 0; i < subtag.depth; i++)
			os << " ";
		os << "|>" << subtag;
	}
	return os;
}
////////////////////////////////////////////////////////////
XMLMessage::XMLMessage()
{

}
XMLTag& XMLMessage::AddTag(const std::string& name)
{
	root_tags.push_back(XMLTag(name));
	return root_tags.back();
}
XMLTag& XMLMessage::AddTag(const std::string& name, const std::string& value)
{
	root_tags.push_back(XMLTag(name, value));
	return root_tags.back();
}
XMLTag &XMLMessage::FindTag(const std::string &name)
{
	for (XMLTag& tag : root_tags)
	{
		if (tag.getName() == name)
			return tag;

		XMLTag& res = tag.FindTag(name);
		if (res.getName() == name)
			return res;
	}
	throw std::runtime_error("tag not found");
}
std::string XMLMessage::toString() const
{
	std::stringstream res;
	for (const XMLTag& tag : root_tags)
		res << tag.toXML();
	return res.str();
}
XMLMessage::operator std::string() const
{
	return toString();
}
std::ostream& operator<<(std::ostream& os, const XMLMessage& xml)
{
	for (XMLTag tag : xml.root_tags)
		os << tag;
	return os;
}
////////////////////////////////////////////////////////////
XMLMessage XMLParser::parseXMLString(const std::string& str)
{
	XMLMessage res;
	RawXML raw;

	raw.parseString(str);
	raw.finalize();

	std::vector<std::string> open_tags;
	XMLTag* last_tag = nullptr;
	for (const RawTag& rawtag : raw.tags)
	{
		if (rawtag._CLOSING_TAG_)
		{
			if (last_tag != nullptr)
				last_tag = last_tag->getParentTag();
			for (std::vector<std::string>::const_iterator iter = open_tags.cbegin(); iter != open_tags.cend();)
			{
				if ("/" +  *iter == rawtag.name)
				{
					iter = open_tags.erase(iter);
					break;
				}
				else
					iter++;
			}
			continue;
		}
		else if (rawtag._SELF_CLOSING_TAG_)
		{
			XMLTag* self_close = nullptr;
			if (last_tag != nullptr)
				self_close = &last_tag->AddTag(rawtag.name);
			else
				self_close = &res.AddTag(rawtag.name);
			for (const RawAttribute& attr : rawtag.attributes)
				self_close->AddAttribute(attr.name, attr.value);
			continue;
		}
		else if (rawtag._PROC_INST_)
		{
			XMLTag* proc = nullptr;
			if (last_tag != nullptr)
				proc = &last_tag->AddTag(rawtag.name);
			else
				proc = &res.AddTag(rawtag.name);
			proc->makeProcInstruction();
			for (const RawAttribute& attr : rawtag.attributes)
				proc->AddAttribute(attr.name, attr.value);
			continue;
		}
		if (last_tag == nullptr)
		{
			last_tag = &res.AddTag(rawtag.name, rawtag.value);
			open_tags.emplace_back(rawtag.name);
			for (const RawAttribute& attr : rawtag.attributes)
				last_tag->AddAttribute(attr.name, attr.value);
		}
		else
		{
			last_tag = &last_tag->AddTag(rawtag.name, rawtag.value);
			open_tags.emplace_back(rawtag.name);
			for (const RawAttribute& attr : rawtag.attributes)
				last_tag->AddAttribute(attr.name, attr.value);
		}
	}
	if (open_tags.size() != 0)
		throw std::runtime_error("XML syntax error: couldn't find valid pair of open and close tags");
	return res;
}
void XMLParser::RawTag::parseTagContent()
{
	bool _PROC_ = false;
	bool _EQUALS_ = false;

	ParserState state = START;
	std::string parse_out;

	for (uint64_t iterator = 0; iterator < content.size(); iterator++)
	{
		char token = content[iterator];

		switch (token)
		{
		case(' '):
			switch (state)
			{
			case (TAG_NAME):
				state = EXPECT_ATTRIBUTE;
				name = parse_out;
				parse_out.clear();
				break;
			case (ATTRIBUTE_NAME):
				state = EXPECT_VALUE;
				attributes.emplace_back(RawAttribute({ .name = parse_out }));
				parse_out.clear();
				break;
			case (ATTRIBUTE_VALUE):
				parse_out.append(1, token);
				break;
			default:
				continue;
				break;
			}
			break;
		case ('='):
			switch (state)
			{
			case (ATTRIBUTE_NAME):
				state = EXPECT_VALUE;
				_EQUALS_ = true;
				attributes.emplace_back(RawAttribute({ .name = parse_out }));
				parse_out.clear();
				break;
			case (ATTRIBUTE_VALUE):
				parse_out.append(1, token);
				break;
			case (EXPECT_VALUE):
				if (!_EQUALS_)
					_EQUALS_ = true;
				else
					throw std::runtime_error("XML syntax error: unexpected '='");
				break;
			default:
				throw std::runtime_error("XML syntax error: unexpected '='");
			}
			break;
		case ('"'):
			switch (state)
			{
			case (EXPECT_VALUE):
				if (_EQUALS_)
					state = ATTRIBUTE_VALUE;
				else
					throw std::runtime_error("XML syntax error: unexpected '\"'");
				break;
			case (ATTRIBUTE_VALUE):
				if (_EQUALS_)
				{
					state = EXPECT_ATTRIBUTE;
					attributes.back().value = parse_out;
					parse_out.clear();
					_EQUALS_ = false;
				}
				else
					throw std::runtime_error("XML syntax error: unexpected '\"'");
				break;
			default:
				throw std::runtime_error("XML syntax error: unexpected '\"'");
				break;
			}
			break;
		case ('/'):
			switch (state)
			{
			case (START):
				parse_out.append(1, token);
				_CLOSING_TAG_ = true;
				break;
			case (TAG_NAME):
				if (!_SELF_CLOSING_TAG_ && !_PROC_ && !_CLOSING_TAG_)
				{
					name = parse_out;
					parse_out.clear();
					_SELF_CLOSING_TAG_ = true;
				}
				else
					throw std::runtime_error("XML syntax error: unexpected '/'");
				break;
			case (EXPECT_ATTRIBUTE):
				if (!_SELF_CLOSING_TAG_ && !_PROC_ && !_CLOSING_TAG_)
					_SELF_CLOSING_TAG_ = true;
				else
					throw std::runtime_error("XML syntax error: unexpected '/'");
				break;
			case (ATTRIBUTE_VALUE):
				parse_out.append(1, token);
				break;
			default:
				throw std::runtime_error("XML syntax error: unexpected '/'");
				break;
			}
			break;
		case ('?'):
			switch (state)
			{
			case (START):
				_PROC_ = true;
				break;
			case (ATTRIBUTE_VALUE):
				parse_out.append(1, token);
				break;
			case (TAG_NAME):
				if (_PROC_ && !_SELF_CLOSING_TAG_)
				{
					name = parse_out;
					parse_out.clear();
					_PROC_INST_ = true;
					_PROC_ = false;
				}
				else
					throw std::runtime_error("XML syntax error: unexpected '?'");
				break;
			case (EXPECT_ATTRIBUTE):
				if (_PROC_ && !_SELF_CLOSING_TAG_)
				{
					_PROC_INST_ = true;
					_PROC_ = false;
				}
				else
					throw std::runtime_error("XML syntax error: unexpected '?'");
				break;
			default:
				throw std::runtime_error("XML syntax error: unexpected '?'");
				break;
			}
			break;
		default:
			switch (state)
			{
			case (START):
				state = TAG_NAME;
				parse_out.append(1, token);
				break;
			case (EXPECT_ATTRIBUTE):
				state = ATTRIBUTE_NAME;
				parse_out.append(1, token);
				break;
			case (TAG_NAME): [[fallthrough]];
			case (ATTRIBUTE_NAME): [[fallthrough]];
			case (ATTRIBUTE_VALUE):
				parse_out.append(1, token);
				break;
			default:
				throw std::runtime_error("XML syntax error: unexpected text");
				break;
			}
			break;
		}
	}
	switch (state)
	{
	case (EXPECT_ATTRIBUTE):
		state = END;
		break;
	case (TAG_NAME):
		state = END;
		if (!_PROC_INST_ && !_SELF_CLOSING_TAG_)
		{
			name = parse_out;
			parse_out.clear();
		}
		break;
	default:
		throw std::runtime_error("XML parser error: invalid syntax");
		break;
	}
	if (_PROC_)
		throw std::runtime_error("XML parser error: processing instruction was not closed by '?'");
	if (_EQUALS_)
		throw std::runtime_error("XML parser error: attribute syntax error with symbol '='");
}
bool XMLParser::RawValue::Validate()
{
	if (before == nullptr || after == nullptr)
		return false;
	if (before->name.find('/') == std::string::npos && after->name.find('/') == 0)
	{
		before->value = content;
		return true;
	}
	else
		return false;
}
void XMLParser::RawXML::parseString(const std::string& string)
{
	ParserState state = START;
	std::string parse_out;

	for (uint64_t iterator = 0; iterator < string.size(); iterator++)
	{
		char token = string[iterator];

		switch (token)
		{
		case ('<'):
			switch (state)
			{
			case (START):
				state = TAG;
				break;
			case (VALUE):
				state = TAG;
				values.emplace_back(RawValue({ .content = parse_out, .before = &tags.back() }));
				parse_out.clear();
				break;
			default:
				throw std::runtime_error("XML syntax error: unexpected '<'");
				break;
			}
			break;
		case ('>'):
			switch (state)
			{
			case (TAG):
				state = VALUE;
				tags.emplace_back(RawTag({ .content = parse_out }));
				if (!values.empty()) values.back().after = &tags.back();
				parse_out.clear();
				break;
			case (END):
				continue;
				break;
			default:
				throw std::runtime_error("XML syntax error: unexpected '>'");
				break;
			}
			break;
		case(' '):
			switch (state)
			{
			case (TAG):
				parse_out.append(1, token);
				break;
			case (VALUE):
				parse_out.append(1, token);
				break;
			default:
				continue;
				break;
			}
			break;
		case ('\n'):
		case ('\r'):
		case ('\t'):
			continue;
			break;
		default:
			switch (state)
			{
			case (TAG):
				parse_out.append(1, token);
				break;
			case (VALUE):
				parse_out.append(1, token);
				break;
			default:
				throw std::runtime_error("XML syntax error: misplaced text");
				break;
			}
			break;
		}
	}
	switch (state)
	{
	case (VALUE):
		state = END;
		break;
	default:
		throw std::runtime_error("XML parser error: invalid syntax");
		break;
	}
}
void XMLParser::RawXML::finalize()
{
	for (RawTag& tag : tags)
		tag.parseTagContent();
	for (std::list<RawValue>::iterator iter = values.begin(); iter != values.end();)
	{
		if (!iter->Validate())
			iter = values.erase(iter);
		else
			iter++;
	}
}
////////////////////////////////////////////////////////////
