////////////////////////////////////////////////////////////
/*
* basic XML-Parser
* 
* supports:
* - 'standard' XML (tag and value pairs, optionally attributes)
* - self-closing tags (<tag/>), 
* - processing instructions (<?xml...?>)
* 
* use XMLParser::parseXMLString() to parse a string into an XMLTree
* use XMLTree and XMLTree::AddTag() and the returned XMLTag& as well as it's toString() to construct an XML-message
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
////////////////////////////////////////////////////////////
namespace xml
{
	////////////////////////////////////////////////////////////
	/*
	* XMLAttribute class
	* --------------------
	* stores name and value of a XML attribute
	*/
	////////////////////////////////////////////////////////////
	class XMLAttribute
	{
	public:
		XMLAttribute() = default;
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
	////////////////////////////////////////////////////////////
	/*
	* XMLTag class
	* --------------------
	* stores name and value of a XML Tag as well as its subtags
	*/
	////////////////////////////////////////////////////////////
	class XMLTag
	{
	public:
		XMLTag() = default;
		XMLTag(const std::string& name);
		XMLTag(const std::string& name, const std::string& value);

		std::string getName() const;
		std::string getValue() const;
		std::vector<refw(XMLTag)> getSubTags();
		std::vector<refw(XMLAttribute)> getAttributes();
		XMLTag& getParentTag();
		std::vector<refw(XMLTag)> FindTags(const std::string& name);

		void setName(const std::string& str);
		void setValue(const std::string& str);
		void setProcInstruction(bool val);
		void setParentTag(XMLTag& tag);

		XMLTag& AddTag(const std::string& name);
		XMLTag& AddTag(const std::string& name, const std::string& value);
		XMLTag& AddTag(const XMLTag& subtag);

		XMLAttribute& AddAttribute(const std::string& name, const std::string& value);
		XMLAttribute& AddAttribute(const XMLAttribute& atr);
		/*
		* accepts name-value pairs of strings as multiple attributes to be added at once
		* 
		* returns all attributes of the tag
		*/
		std::vector<refw(XMLAttribute)> AddAttributes(const std::vector<std::string>& value);
		std::vector<refw(XMLAttribute)> AddAttributes(const std::vector<XMLAttribute>& value);

		std::string toXML() const;
	private:
		friend class XMLTree;
		friend std::ostream& operator<<(std::ostream& os, const XMLTag& tag);

		std::string name;
		std::string value;
		bool _PROC_ = false;
		uint64_t depth = 0;

		XMLTag* parent = nullptr;
		std::list<XMLAttribute> attributes;
		std::list<XMLTag> subtags;
	};
	std::ostream& operator<<(std::ostream& os, const XMLTag& tag);
	////////////////////////////////////////////////////////////
	/*
	* XMLTree class
	* --------------------
	* represents a complete XML document tree
	* can be used to process or construct XML documents
	*/
	////////////////////////////////////////////////////////////
	class XMLTree
	{
	public:
		XMLTree() = default;
		std::vector<refw(XMLTag)> FindTags(const std::string& name);

		XMLTag& AddTag(const std::string& name);
		XMLTag& AddTag(const std::string& name, const std::string& value);

		std::string to_string() const;
		operator std::string() const;
	private:
		friend std::ostream& operator<<(std::ostream& os, const XMLTree& xml);

		std::list<XMLTag> root_tags;
	};
	std::ostream& operator<<(std::ostream& os, const XMLTree& xml);
	////////////////////////////////////////////////////////////
	/*
	* XMLParser class
	* --------------------
	* used to convert XML data as strings to XMLTree data structures
	*/
	////////////////////////////////////////////////////////////
	class XMLParser
	{
	public:
		XMLParser() = delete;
		[[nodiscard]] static XMLTree parseString(const std::string& str);

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
}
////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////
