////////////////////////////////////////////////////////////
#if __has_include("XMLParser.hpp")
#include "XMLParser.hpp"
#endif
////////////////////////////////////////////////////////////
#ifdef XML_PARSER_H
////////////////////////////////////////////////////////////
XMLAttribute::XMLAttribute(std::string _name, std::string _val) : name(_name), value(_val)
{

}
std::string& XMLAttribute::getName()
{
	return name;
}
std::string& XMLAttribute::getValue()
{
	return value;
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
std::string& XMLTag::getName()
{
	return name;
}
std::string& XMLTag::getValue()
{
	return value;
}
std::list<XMLTag>& XMLTag::getSubTags()
{
	return subtags;
}
std::list<XMLAttribute>& XMLTag::getAttributes()
{
	return attributes;
}
XMLTag& XMLTag::getParentTag()
{
	if (parent != nullptr)
		return *parent;
	throw std::runtime_error("XMLTag ParentTag was nullptr when trying to access");
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
XMLAttribute& XMLTag::AddAttribute(const std::string& _name, const std::string& _val)
{
	return AddAttribute(XMLAttribute(_name, _val));
}
XMLAttribute& XMLTag::AddAttribute(const XMLAttribute& atr)
{
	attributes.emplace_back(atr);
	return attributes.back();
}
std::string XMLTag::toXML() const
{
	std::stringstream res;
	for (uint64_t i = 0; i < depth; i++)
		res << ' ';
	res << '<' << name;
	for (const XMLAttribute& atr : attributes)
		res << ' ' << atr.toXML();
	res << '>';
	if (subtags.size() != 0)
	{
		for (const XMLTag& tag : subtags)
			res << '\n' << tag.toXML();
	}
	else
		res << value;
	if (subtags.size() != 0)
	{
		res << '\n';
		for (uint64_t i = 0; i < depth; i++)
			res << ' ';
	}
	res << '<' << '/' << name << '>';
	return res.str();
}
std::ostream& operator<<(std::ostream& os, const XMLTag& tag)
{
	if (tag.attributes.size() != 0)
	{
		os << tag.name << " (";
		for (XMLAttribute atr : tag.attributes)
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
XMLTree::XMLTree(const std::string& raw_xml)
{
	parseString(raw_xml);
}
XMLTag& XMLTree::AddTag(const std::string& name)
{
	root_tags.push_back(XMLTag(name));
	return root_tags.back();
}
XMLTag& XMLTree::AddTag(const std::string& name, const std::string& value)
{
	root_tags.push_back(XMLTag(name, value));
	return root_tags.back();
}
std::string XMLTree::toString() const
{
	std::stringstream res;
	for (const XMLTag& tag : root_tags)
		res << tag.toXML();
	return res.str();
}
std::string XMLTree::operator()(std::string) const
{
	return toString();
}
char XMLTree::getToken(std::string& _raw)
{
	char res = _raw[0];
	_raw = _raw.substr(1);
	return res;
}
void XMLTree::parseString(std::string raw_xml)
{
	try
	{
		XMLParserFlag flags = NO_FLAGS;
		XMLParserState phase = IDLE;

		std::string parse_out;
		XMLTag* last_tag = nullptr;

		while (!raw_xml.empty())
		{
			char token = getToken(raw_xml);

			switch (token)
			{
			case ('<'):
			{
				if (phase == READING_VALUE)
					flags = COMPLETED_VALUE;
				phase = OPEN_TAG;
				break;
			}
			case ('>'):
			{
				if (phase == READING_NAME)
					flags = COMPLETED_NAME;
				phase = CLOSE_TAG;
				break;
			}
			case (' '):
			{
				if (phase == READING_NAME)
				{
					flags = COMPLETED_NAME;
					phase = READING_ATTRIBUTE_NAME;
				}
				else if (phase == READING_VALUE)
				{
					parse_out.append(1, token);
				}
				break;
			}
			case ('/'):
			{
				parse_out.append(1, token);
				break;
			}
			case ('='):
			{
				if (phase == READING_ATTRIBUTE_NAME)
				{
					flags = COMPLETED_ATTRIBUTE_NAME;
				}
				break;
			}
			case ('?'):
				continue;
				break;
			case ('\r'):
			case ('\n'):
			case ('\r\n'):
			case ('\t'):
				continue;
				break;
			case ('\"'):
			{
				if (phase == READING_ATTRIBUTE_NAME)
				{
					phase = READING_ATTRIBUTE_VALUE;
				}
				else if (phase == READING_ATTRIBUTE_VALUE)
				{
					flags = COMPLETED_ATTRIBUTE_VALUE;
				}
				break;
			}
			default:
			{
				if (phase == OPEN_TAG)
					phase = READING_NAME;
				if (phase == CLOSE_TAG)
					phase = READING_VALUE;
				if (phase == READING_NAME ||
					phase == READING_VALUE ||
					phase == READING_ATTRIBUTE_NAME ||
					phase == READING_ATTRIBUTE_VALUE)
					parse_out.append(1, token);
				break;
			}
			}

			if (flags == COMPLETED_NAME)
			{
				if (parse_out[0] != '/')
				{
					if (last_tag == nullptr)
					{
						root_tags.emplace_back(XMLTag(parse_out));
						last_tag = &root_tags.back();
					}
					else
					{
						last_tag = &last_tag->AddTag(XMLTag(parse_out));
					}
				}
				else
				{
					try
					{
						last_tag = &last_tag->getParentTag();
					}
					catch (std::exception e) {} // parent tag was nullptr, meaning an error occured or
					                            // the tag was a root tag
				}
				parse_out.clear();
				flags = NO_FLAGS;
			}
			if (flags == COMPLETED_VALUE)
			{
				if (last_tag != nullptr)
					last_tag->getValue() = parse_out;

				parse_out.clear();
				flags = NO_FLAGS;
			}
			if (flags == COMPLETED_ATTRIBUTE_NAME)
			{
				if (last_tag != nullptr)
					last_tag->AddAttribute(XMLAttribute(parse_out, ""));
				parse_out.clear();
				flags = NO_FLAGS;
			}
			if (flags == COMPLETED_ATTRIBUTE_VALUE)
			{
				if (last_tag != nullptr)
					last_tag->getAttributes().back().getValue() = parse_out;
				parse_out.clear();
				flags = NO_FLAGS;
			}
		}
		phase = IDLE;
	}
	catch (std::exception exception)
	{
		std::cerr << "error with parsing XMLTree: " << exception.what() << std::endl;
	}
}
std::ostream& operator<<(std::ostream& os, const XMLTree& xml)
{
	for (XMLTag tag : xml.root_tags)
		os << tag;
	return os;
}
XMLTree parseXMLString(const std::string& raw_xml)
{
	return XMLTree(raw_xml);
}
std::string parseXMLTree(const XMLTree& xml_tree)
{
	return xml_tree.toString();
}
////////////////////////////////////////////////////////////
#else
#error XMLTree parser source-file could not compile. missing 'XMLTree.hpp'-file.
#endif
////////////////////////////////////////////////////////////
