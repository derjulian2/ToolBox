////////////////////////////////////////////////////////////
/*
* basic XML-Parser
* 
* supports regular XML (tag and value pairs), self-closing tags, 
* processing instructions (?xml...) and attributes
* 
* use XMLParser::parseXMLString() to parse a string into an XMLMessage
* use XMLMessage and XMLMessage::AddTag() and the returned XMLTag& as well as it's toString() to construct an XML-message
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
* stores name and value of an XMLMessage-attribute
*/
class XMLAttribute
{
public:
	XMLAttribute(std::string _name, std::string _val);

	std::string getName() const;
	void setName(const std::string& str);
	std::string getValue() const;
	void setValue(const std::string& str);
	std::string toXML() const;
private:
	friend std::ostream& operator<<(std::ostream& os, const XMLAttribute& atr);

	std::string name;
	std::string value;
};
std::ostream& operator<<(std::ostream& os, const XMLAttribute& atr);
/*
* XMLTag class
* stores name and value or subtags of a corresponding XMLMessage-tag
*/
class XMLTag
{
public:
	XMLTag(const std::string& _name);
	XMLTag(const std::string& _name, const std::string& _val);

	std::string getName() const;
	void setName(const std::string& str);
	std::string getValue() const;
	void setValue(const std::string& str);
	std::list<XMLTag>& getSubTags();
	std::list<XMLAttribute>& getAttributes();
	void makeProcInstruction();
	void removeProcInstruction();
	XMLTag* getParentTag();
	void setParentTag(XMLTag& tag);

	XMLTag& AddTag(const std::string& _name);
	XMLTag& AddTag(const std::string& _name, const std::string& _val);
	XMLTag& AddTag(const XMLTag& subtag);

	XMLAttribute& AddAttribute(const std::string& _name, const std::string& _val);
	XMLAttribute& AddAttribute(const XMLAttribute& atr);
	// accepts name-value pairs of string as multiple attributes
	std::list<XMLAttribute>& AddAttributes(const std::vector<std::string>& _val);
	std::list<XMLAttribute>& AddAttributes(const std::vector<XMLAttribute>& _val);

	std::string toXML() const;
private:
	friend std::ostream& operator<<(std::ostream& os, const XMLTag& tag);

	std::string name;
	std::string value;
	uint64_t depth = 0;

	std::list<XMLAttribute> attributes;
	std::list<XMLTag> subtags;

	XMLTag* parent = nullptr;
	bool _PROC_ = false;
};
std::ostream& operator<<(std::ostream& os, const XMLTag& tag);
/*
* XMLMessage class
* stores a complete XML-message-tree
* used to construct custom XML-messages
*/
class XMLMessage
{
public:
	XMLMessage();

	std::string toString() const;
	operator std::string() const;

	XMLTag& AddTag(const std::string& name);
	XMLTag& AddTag(const std::string& name, const std::string& value);
private:
	friend std::ostream& operator<<(std::ostream& os, const XMLMessage& xml);

	std::list<XMLTag> root_tags;
};
std::ostream& operator<<(std::ostream& os, const XMLMessage& xml);
/*
* XMLParser class
* used to convert XML-strings to XMLMessage data structures
*/
class XMLParser
{
public:
	XMLParser();
	
	XMLMessage parseXMLString(const std::string& str);

private:

	struct RawAttribute
	{
		std::string name;
		std::string value;
	};

	struct RawTag
	{
		std::string content;

		void parseTagContent();

		bool _PROC_INST_ = false;
		bool _CLOSING_TAG_ = false;
		bool _SELF_CLOSING_TAG_ = false;
		std::string name;
		std::string value;
		std::list<RawAttribute> attributes;

		enum ParserState
		{
			START,
			TAG_NAME,
			EXPECT_ATTRIBUTE,
			ATTRIBUTE_NAME,
			EXPECT_VALUE,
			ATTRIBUTE_VALUE,
			END
		};
	};

	struct RawValue
	{
		std::string content;

		bool Validate();

		RawTag* before = nullptr;
		RawTag* after = nullptr;
	};

	struct RawXML
	{
		void parseString(const std::string& string);
		void finalize();

		std::list<RawTag> tags;
		std::list<RawValue> values;

		enum ParserState
		{
			START,
			TAG,
			VALUE,
			END
		};
	};
};
////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////
