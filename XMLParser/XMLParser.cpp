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
	// print depth indentation
	for (uint64_t i = 0; i < depth; i++)
		res << ' ';
	// print opening tag and attributes
	res << '<' << name;
	for (const XMLAttribute& atr : attributes)
		res << ' ' << atr.toXML();
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
void XMLTree::parseString(const std::string& raw_xml)
{
	char EXPECTING = '\0';
	char PARSING = '\0';
	bool __CLOSE_TAG__ = false;

	XMLParserState state = IDLE;
	uint64_t iterator = 0;
	XMLTag* last_tag = nullptr;

	std::vector<std::string> open_tags;

	std::string parse_out;

	char token = '\0';
	while (iterator != raw_xml.length())
	{
		token = raw_xml[iterator];
		iterator++;

		switch (token)
		{
		case ('<'):
			if (PARSING == 'V')
				state = VAL_FINISHED;

			EXPECTING = 'N';
			break;
		case ('>'):
			if (PARSING == 'N')
				state = NAME_FINISHED;
			else if (EXPECTING == 'A')
				EXPECTING = 'V';
			else
				state = ERROR;

			break;
		case (' '):

			if (PARSING == 'N')
				state = NAME_FINISHED;
			else if (PARSING == 'A')
				state = ATTR_NAME_FINISHED;
			else if (PARSING == 'P')
				parse_out.append(1, token);

			break;
		case ('/'):
			if (EXPECTING == 'N' && !__CLOSE_TAG__)
				__CLOSE_TAG__ = true;
			else
				state = ERROR;
			break;
		case ('='):
			if (PARSING == 'A')
				state = ATTR_NAME_FINISHED;
			else
				state = ERROR;
			break;
		case ('\"'):
			if (EXPECTING == 'P')
			{
				PARSING = 'P';
				EXPECTING = '\0';
			}
			else if (PARSING == 'P')
				state = ATTR_VAL_FINISHED;
			else
				state = ERROR;


			break;
		case ('?'):
		case ('\r'):
		case ('\n'):
		case ('\r\n'):
		case ('\t'):
			continue;
			break;
		default:
			if (EXPECTING == 'N')
			{
				PARSING = 'N';
				EXPECTING = '\0';
			}
			if (EXPECTING == 'A')
			{
				PARSING = 'A';
				EXPECTING = '\0';
			}
			if (EXPECTING == 'V')
			{
				PARSING = 'V';
				EXPECTING = '\0';
			}
			parse_out.append(1, token);
			break;
		}

		switch (state)
		{
		case (NAME_FINISHED):
			if (!__CLOSE_TAG__)
			{
				open_tags.push_back(parse_out);
				if (last_tag == nullptr)
					last_tag = &root_tags.emplace_back(XMLTag(parse_out));
				else
					last_tag = &last_tag->AddTag(parse_out);
			}
			else
			{
				if (open_tags[0] == parse_out)
					open_tags.erase(open_tags.begin());
				else
					throw std::runtime_error("error with parsing XML-string");
				try
				{
					last_tag = &last_tag->getParentTag();
				}
				catch (std::exception e)
				{
					last_tag = nullptr;
				}
				__CLOSE_TAG__ = false;
			}
			parse_out.clear();
			state = IDLE;


			EXPECTING = 'A';
			PARSING = '\0';
			break;
		case (VAL_FINISHED):
			last_tag->getValue() = std::string(parse_out);
			parse_out.clear();
			state = IDLE;

			if (PARSING == 'V')
				EXPECTING = 'N';
			else
				EXPECTING = '\0';
			PARSING = '\0';
			break;
		case (ATTR_NAME_FINISHED):
			last_tag->AddAttribute(parse_out, "");
			parse_out.clear();
			state = IDLE;
			PARSING = '\0';
			EXPECTING = 'P';
			break;
		case (ATTR_VAL_FINISHED):
			last_tag->getAttributes().back().getValue() = parse_out;
			parse_out.clear();
			state = IDLE;
			PARSING = '\0';
			EXPECTING = 'A';
			break;
		case (ERROR):
			throw std::runtime_error("error with parsing XML-string");
			break;
		}
	}
	if (open_tags.size() != 0)
		throw std::runtime_error("error with parsing XML-string");
	state = FINISHED;
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
