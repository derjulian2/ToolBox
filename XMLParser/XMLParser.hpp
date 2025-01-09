////////////////////////////////////////////////////////////
/*
* rudimentary XMLTree-parser
* - stores a XMLTree-tag as 'XMLTag'
* - Subtags are stored in XMLTag::SubTags
* - stores XMLTree-attributes in 'XMLAttribute'
* - stores a whole XMLTree-tree in 'XMLTree'
* - all classes are printable using std::ostream& << operator
* 
* - currently supports no error detection whatsoever. if your xml-file is faulty or the parse
*   does not recognize it, errors in your data will occur.
* - beware that adding a subtag to a tag erases its value. data may be lost that way.
* 
* use XMLTree::parseXMLString() to parse a raw string
* use XMLTree::parseXMLtoString() to parse XMLTree-class to a string
* use XMLTree-class and XMLTree::AddTag() / XMLTree::AddAttribute() to construct an XMLTree-message
*/
////////////////////////////////////////////////////////////
#ifndef XML_PARSER_H
#define XML_PARSER_H
////////////////////////////////////////////////////////////
#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <vector>
////////////////////////////////////////////////////////////
/*
* XMLAttribute class
* stores name and value of an XMLTree-attribute
*/
class XMLAttribute
{
public:
	XMLAttribute(std::string _name, std::string _val);

	std::string& getName();
	std::string& getValue();
	std::string toXML() const;
private:
	friend std::ostream& operator<<(std::ostream& os, const XMLAttribute& atr);

	std::string name;
	std::string value;
};
std::ostream& operator<<(std::ostream& os, const XMLAttribute& atr);
/*
* XMLTag class
* stores name and value or subtags of a corresponding XMLTree-tag
*/
class XMLTag
{
public:
	XMLTag(const std::string& _name);
	XMLTag(const std::string& _name, const std::string& _val);

	std::string& getName();
	std::string& getValue();
	std::list<XMLTag>& getSubTags();
	std::list<XMLAttribute>& getAttributes();
	XMLTag& getParentTag();
	void setParentTag(XMLTag& tag);

	XMLTag& AddTag(const std::string& _name);
	XMLTag& AddTag(const std::string& _name, const std::string& _val);
	XMLTag& AddTag(const XMLTag& subtag);

	XMLAttribute& AddAttribute(const std::string& _name, const std::string& _val);
	XMLAttribute& AddAttribute(const XMLAttribute& atr);

	std::string toXML() const;
private:
	friend std::ostream& operator<<(std::ostream& os, const XMLTag& tag);

	std::string name;
	std::string value;
	uint64_t depth = 0;

	std::list<XMLAttribute> attributes;
	std::list<XMLTag> subtags;

	XMLTag* parent = nullptr;

};
std::ostream& operator<<(std::ostream& os, const XMLTag& tag);
/*
* XMLTree class
* stores a complete XMLTree-tree
*/
class XMLTree
{
public:
	XMLTree(const std::string& raw_xml);

	std::string toString() const;
	std::string operator()(std::string) const;

	XMLTag& AddTag(const std::string& name);
	XMLTag& AddTag(const std::string& name, const std::string& value);
private:
	friend std::ostream& operator<<(std::ostream& os, const XMLTree& xml);
	/*
	* XMLTree-parser state-indicators
	* intended for internal parser-processes only
	*/
	enum XMLParserState
	{
		IDLE,
		FINISHED,
		ERROR,

		NAME_FINISHED,
		VAL_FINISHED,
		ATTR_NAME_FINISHED,
		ATTR_VAL_FINISHED
	};

	void parseString(const std::string& _raw);

	std::list<XMLTag> root_tags;
};
std::ostream& operator<<(std::ostream& os, const XMLTree& xml);

XMLTree parseXMLString(const std::string& raw_xml);
std::string parseXMLTree(const XMLTree& xml_tree);
////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////
