////////////////////////////////////////////////////////////
/*
* basic XML-Parser
* 
* supports:
* - 'standard' XML (tag and value pairs, optionally attributes)
* - self-closing tags (<tag/>), 
* - processing instructions (<?xml...?>)
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
#include <functional>
#include <vector>
////////////////////////////////////////////////////////////
#define refw(type) std::reference_wrapper<type>
namespace xmlp
{
	/*
	* XMLAttribute class
	* --------------------
	* stores name and value of a XML attribute
	*/
	class XMLAttribute
	{
	public:
		XMLAttribute(std::string name, std::string value);

		std::string getName() const;
		std::string getValue() const;

		void setName(const std::string& str);
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
	* --------------------
	* stores name and value of a XML Tag as well as its subtags
	*/
	class XMLTag
	{
	public:
		XMLTag(const std::string& name);
		XMLTag(const std::string& name, const std::string& value);

		std::string getName() const;
		std::string getValue() const;
		std::vector<refw(XMLTag)> getSubTags() const;
		std::vector<refw(XMLAttribute)> getAttributes() const;
		XMLTag& getParentTag() const;
		std::vector<refw(XMLTag)> FindTags(const std::string& name) const;

		void setName(const std::string& str);
		void setValue(const std::string& str);
		void setProcInstruction(bool val);
		void setParentTag(XMLTag& tag);

		XMLTag& AddTag(const std::string& name);
		XMLTag& AddTag(const std::string& name, const std::string& value);
		XMLTag& AddTag(const XMLTag& subtag);

		XMLAttribute& AddAttribute(const std::string& name, const std::string& value);
		XMLAttribute& AddAttribute(const XMLAttribute& atr);
		// accepts name-value pairs of string as multiple attributes
		std::vector<refw(XMLAttribute)> AddAttributes(const std::vector<std::string>& value);
		std::vector<refw(XMLAttribute)> AddAttributes(const std::vector<XMLAttribute>& value);

		std::string toXML() const;
	private:
		friend class XMLMessage;
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
		std::vector<refw(XMLTag)> FindTags(const std::string& name) const;

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
		[[nodiscard]] static XMLMessage parseXMLString(const std::string& str);

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
}
////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////
