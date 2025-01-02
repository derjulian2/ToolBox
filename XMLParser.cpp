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
std::ostream& operator<<(std::ostream& os, const XMLAttribute& atr)
{
	os << atr.name << "='" << atr.value << "'";
	return os;
}
////////////////////////////////////////////////////////////
XMLNode::XMLNode(std::string _name) : name(_name)
{

}
XMLNode::XMLNode(std::string _name, std::string _val) : name(_name), value(_val)
{

}
XMLNode& XMLNode::AddNode(const XMLNode& subnode)
{
	SubNodes.emplace_back(subnode);
	SubNodes.back().depth = depth + 1;
	SubNodes.back().parent = this;
	value.clear();
	return SubNodes.back();
}
XMLAttribute& XMLNode::AddAttribute(const XMLAttribute& atr)
{
	Attributes.emplace_back(atr);
	return Attributes.back();
}
std::ostream& operator<<(std::ostream& os, const XMLNode& node)
{
	if (node.Attributes.size() != 0)
	{
		os << node.name << " (";
		for (uint16_t _iter = 0; _iter < node.Attributes.size(); _iter++)
		{
			os << node.Attributes[_iter];
			if (_iter != node.Attributes.size() - 1)
				os << ", ";
		}
		os << "): " << node.value << "\n";
	}
	else
		os << node.name << ": " << node.value << "\n";
	for (XMLNode subnode : node.SubNodes)
	{
		for (uint16_t i = 0; i < subnode.depth; i++)
			os << " ";
		os << static_cast<char>(192) << subnode;
	}
	return os;
}
////////////////////////////////////////////////////////////
constexpr XMLphase operator|(const XMLphase& _v, const XMLphase& _w)
{
	return static_cast<XMLphase>(static_cast<uint16_t>(_v) | static_cast<uint16_t>(_w));
}
////////////////////////////////////////////////////////////
XML XML::parseXMLString(std::string _raw)
{
	XML xmlout;
	format(_raw);

	try
	{
		XMLflag flags = NO_FLAGS;
		XMLphase phase = IDLE;

		std::string parse_out;
		XMLNode* last_node = nullptr;

		while (!_raw.empty())
		{
			char token = getToken(_raw);

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
					if (last_node == nullptr)
					{
						xmlout.Nodes.push_back(XMLNode(parse_out));
						last_node = &xmlout.Nodes.back();
					}
					else
					{
						last_node = &last_node->AddNode(XMLNode(parse_out));
					}
				}
				else
				{
					last_node = last_node->parent;
				}
				parse_out.clear();
				flags = NO_FLAGS;
			}
			if (flags == COMPLETED_VALUE)
			{
				if (last_node != nullptr)
					last_node->value = parse_out;

				parse_out.clear();
				flags = NO_FLAGS;
			}
			if (flags == COMPLETED_ATTRIBUTE_NAME)
			{
				if (last_node != nullptr)
					last_node->AddAttribute(XMLAttribute(parse_out, ""));
				parse_out.clear();
				flags = NO_FLAGS;
			}
			if (flags == COMPLETED_ATTRIBUTE_VALUE)
			{
				if (last_node != nullptr)
					last_node->Attributes.back().value = parse_out;
				parse_out.clear();
				flags = NO_FLAGS;
			}
		}
		phase = IDLE;
	}
	catch (std::exception exception)
	{
		std::cerr << "error with parsing XML: " << exception.what() << std::endl;
	}
	return xmlout;
}
std::string XML::parseXMLtoString(XML xml)
{
	std::stringstream res;
	for (XMLNode node : xml.Nodes)
	{
		res << NodetoString(node);
	}
	return res.str();
}
XMLNode& XML::AddNode(std::string name)
{
	Nodes.push_back(XMLNode(name));
	return Nodes.back();
}
XMLNode& XML::AddNode(std::string name, std::string value)
{
	Nodes.push_back(XMLNode(name, value));
	return Nodes.back();
}
char XML::getToken(std::string& _raw)
{
	char res = _raw[0];
	_raw = _raw.substr(1);
	return res;
}
std::string XML::format(std::string& _i)
{
	std::regex regex("\n|\t|\r\n|\r");
	std::stringstream res;
	res << std::regex_replace(_i, regex, "");
	_i = res.str();
	return res.str();
}
std::string XML::NodetoString(const XMLNode& node)
{
	std::stringstream res;
	for (uint16_t i = 0; i < node.depth; i++)
		res << "  ";
	res << "<" << node.name;
	if (node.Attributes.size() != 0)
	{
		for (XMLAttribute atr : node.Attributes)
		{
			res << " " << atr.name << "=\"" << atr.value << "\"";
		}
	}
	res << ">";
	if (node.SubNodes.size() != 0)
	{
		for (XMLNode subnode : node.SubNodes)
			res << "\n" << NodetoString(subnode) << "\n";
		for (uint16_t i = 0; i < node.depth; i++)
			res << "  ";
	}
	else
	{
		res << node.value;
	}
	res << "</" << node.name << ">";
	return res.str();
}
std::ostream& operator<<(std::ostream& os, const XML& xml)
{
	for (XMLNode node : xml.Nodes)
		os << node;
	return os;
}
////////////////////////////////////////////////////////////
#else
#error XML parser source-file could not compile. missing 'XMLParser.hpp'-file.
#endif
////////////////////////////////////////////////////////////