#include <string>
#include <iostream>
#include <cstdint>
#include <list>
#include <algorithm>

struct RawAttribute
{
	std::string name;
	std::string value;
};

struct RawTag
{
	std::string content;

	// breaks up raw content into name and attributes
	void parseContent()
	{
		bool _START_ = true;
		bool _NAME_OPEN_ = false;

		bool _ATT_EXPC_ = false;
		bool _ATT_OPEN_ = false;

		bool _VAL_EXPC_ = false;
		bool _VAL_OPEN_ = false;

		std::string parse_out;

		for (uint64_t iterator = 0; iterator < content.size(); iterator++)
		{
			char token = content[iterator];

			switch (token)
			{
			case(' '):
				if (_VAL_OPEN_)
					parse_out.append(1, token);
				else if (_NAME_OPEN_)
				{
					_NAME_OPEN_ = false;
					_ATT_EXPC_ = true;
					name = parse_out;
					parse_out.clear();
				}
				else if (_ATT_OPEN_)
				{
					_ATT_OPEN_ = false;
					attributes.emplace_back(RawAttribute({.name = parse_out}));
					parse_out.clear();
				}
				break;
			case ('='):
				if (_VAL_OPEN_)
				{
					parse_out.append(1, token);
				}
				if (!_VAL_OPEN_ && !_VAL_EXPC_)
				{
					_VAL_EXPC_ = true;
					if (_ATT_OPEN_)
					{
						_ATT_OPEN_ = false;
						attributes.emplace_back(RawAttribute({ .name = parse_out }));
						parse_out.clear();
						
					}
				}
				else
					throw std::runtime_error("XML syntax error: unexpected '='");
				break;
			case ('"'):
				if (_VAL_EXPC_ && !_VAL_OPEN_)
				{
					_VAL_EXPC_ = false;
					_VAL_OPEN_ = true;
				}
				else if (!_VAL_EXPC_ && _VAL_OPEN_)
				{
					_VAL_OPEN_ = false;
					_ATT_EXPC_ = true;
					attributes.back().value = parse_out;
					parse_out.clear();
				}
				else
					throw std::runtime_error("XML syntax error: unexpected '\"'");
				break;
			case ('!'):
			case ('/'):
				if (_START_ || _VAL_OPEN_)
					parse_out.append(1, token);
				else
					throw std::runtime_error("XML syntax error: unexpected '/'");
				break;
			default:
				if (_START_)
				{
					_START_ = false;
					_NAME_OPEN_ = true;
				}
				if (_ATT_EXPC_)
				{
					_ATT_EXPC_ = false;
					_ATT_OPEN_ = true;
				}
				if (_NAME_OPEN_ || _ATT_OPEN_ || _VAL_OPEN_)
					parse_out.append(1, token);
				else
					throw std::runtime_error("XML syntax error: unexpected text");
				if (_NAME_OPEN_ && iterator == content.size() - 1)
					name = parse_out;
				break;
			}
		}
	}

	std::string name;
	std::string value;
	std::list<RawAttribute> attributes;
};

struct RawValue
{
	std::string content;

	// discards false values (that occur between two opening tags)
	bool validate() const
	{
		if (before->name.find('/') == std::string::npos && after->name.find('/') == 0)
		{
			before->value = content;
			return true;
		}
		else
			return false;
	}

	RawTag* before = nullptr;
	RawTag* after = nullptr;
};

struct RawXML
{
	void finalize()
	{
		for (RawTag& tag : tags)
			tag.parseContent();
		for (std::list<RawValue>::const_iterator iter = values.cbegin(); iter != values.cend();)
		{
			if (!iter->validate())
				iter = values.erase(iter);
			else
				iter++;
		}
		// finalize tree structure with tag -> value here:
	}

	std::list<RawTag> tags;
	std::list<RawValue> values;
};

RawXML getRawXML(const std::string& data_raw)
{
	RawXML res;

	std::string parse_out;
	bool _START_ = true;
	bool _TAG_OPEN_ = false;
	bool _VAL_OPEN_ = false;

	for (uint64_t iterator = 0; iterator < data_raw.size(); iterator++)
	{
		char token = data_raw[iterator];

		switch (token)
		{
		case ('<'):
			if ((!_TAG_OPEN_ && _VAL_OPEN_) || _START_)
			{
				_TAG_OPEN_ = true;
				_VAL_OPEN_ = false;

				if (_START_) // starts collecting values once the first tag was found
				{
					_START_ = false;
				}
				else
				{
					res.values.emplace_back(RawValue({.content = parse_out, .before = &res.tags.back()}));
					parse_out.clear();
				}
			}
			else
				throw std::runtime_error("XML syntax error: unexpected '<'");
			break;
		case ('>'):
			if (_TAG_OPEN_ && !_VAL_OPEN_)
			{
				_TAG_OPEN_ = false;
				_VAL_OPEN_ = true;
				res.tags.emplace_back(RawTag({.content = parse_out}));
				if (!res.values.empty()) res.values.back().after = &res.tags.back();
				parse_out.clear();
			}
			else
				throw std::runtime_error("XML syntax error: unexpected '>'");
			break;
		case(' '):
			if (_TAG_OPEN_ || _VAL_OPEN_)
				parse_out.append(1, token);
			break;
		default:
			if (_TAG_OPEN_ || _VAL_OPEN_)
				parse_out.append(1, token);
			else
				throw std::runtime_error("XML syntax error: misplaced text");
			break;
		}
	}
	return res;
}



int main() {
	

	return 0;
}