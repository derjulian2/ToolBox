////////////////////////////////////////////////////////////
/*
* rudimentary XML-parser
* - stores a XML-tag as 'XMLNode'
* - Subtags are stored in XMLNode::SubNodes
* - stores XML-attributes in 'XMLAttribute'
* - stores a whole XML-tree in 'XML'
* - all classes are printable using std::ostream& << operator
* 
* - currently supports no error detection whatsoever. if your xml-file is faulty or the parse
*   does not recognize it, errors in your data will occur.
* - beware that adding a subtag to a tag erases its value. data may be lost that way.
* 
* use XML::parseXMLString() to parse a raw string
* use XML::parseXMLtoString() to parse XML-class to a string
* use XML-class and XML::AddNode() / XML::AddAttribute() to construct an XML-message
*/
////////////////////////////////////////////////////////////
#ifndef XML_PARSER_H
#define XML_PARSER_H
////////////////////////////////////////////////////////////
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <regex>
////////////////////////////////////////////////////////////
/*
* XMLAttribute class
* stores name and value of an XML-attribute
*/
class XMLAttribute
{
public:
	XMLAttribute(std::string _name, std::string _val);

	std::string name;
	std::string value;

	friend std::ostream& operator<<(std::ostream& os, const XMLAttribute& atr);
};

std::ostream& operator<<(std::ostream& os, const XMLAttribute& atr);
/*
* XMLNode class
* stores name and value or subtags of a corresponding XML-tag
*/
class XMLNode
{
public:
	XMLNode(std::string _name);
	XMLNode(std::string _name, std::string _val);

	XMLNode& AddNode(const XMLNode& subnode);

	XMLAttribute& AddAttribute(const XMLAttribute& atr);

	friend std::ostream& operator<<(std::ostream& os, const XMLNode& node);

	XMLNode* parent = nullptr;

	std::string name = "";
	std::string value = "";
	uint16_t depth = NULL;

	std::vector<XMLAttribute> Attributes;
	std::vector<XMLNode> SubNodes;

};
std::ostream& operator<<(std::ostream& os, const XMLNode& node);
/*
* XML-parser phase-indicators
* intended for internal parser-processes only
*/
enum XMLphase
{
	IDLE					= 0b0,

	OPEN_TAG				= 0b1,
	CLOSE_TAG				= 0b10,

	READING_NAME			= 0b100,
	READING_ATTRIBUTE_NAME	= 0b1000,
	READING_VALUE			= 0b10000,
	READING_ATTRIBUTE_VALUE = 0b100000

};
constexpr XMLphase operator|(const XMLphase& _v, const XMLphase& _w);
/*
* XML-parser flags to indicate certain events when parsing
* intended for internal parser-processes only
*/
enum XMLflag
{
	NO_FLAGS = 0b0,
	COMPLETED_NAME = 0b1,
	COMPLETED_VALUE = 0b10,
	COMPLETED_ATTRIBUTE_NAME = 0b100,
	COMPLETED_ATTRIBUTE_VALUE = 0b1000,
};
/*
* XML class
* stores a complete XML-tree
*/
class XML
{
public:
	static XML parseXMLString(std::string _raw);
	static std::string parseXMLtoString(XML xml);
	XMLNode& AddNode(std::string name);
	XMLNode& AddNode(std::string name, std::string value);
private:
	static char getToken(std::string& _raw);
	static std::string format(std::string& _i);

	static std::string NodetoString(const XMLNode& node);

	std::vector<XMLNode> Nodes;

	friend std::ostream& operator<<(std::ostream& os, const XML& xml);
};
std::ostream& operator<<(std::ostream& os, const XML& xml);
////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////