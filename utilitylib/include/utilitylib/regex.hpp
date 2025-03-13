////////////////////////////////////////
/// general utility header-only-library
/// for convenience methods/types in C++
/// 2025 Julian Benzel
////////////////////////////////////////
/// string operations around regex
////////////////////////////////////////
#ifndef UTILITYLIB_REGEX_HPP
#define UTILITYLIB_REGEX_HPP
////////////////////////////////////////
#include <string>
#include <regex>
#include "stringmanip.hpp"
////////////////////////////////////////
namespace util
{

	struct regex_match
	{
		std::string content;
		size_t length;
		size_t index;
	};

	static inline std::vector<regex_match> get_matches(const std::string& input, const std::regex& regex)
	{
		std::vector<regex_match> res;
		std::smatch matches;

		std::string::const_iterator start = input.cbegin();
		while (std::regex_search(start, input.cend(), matches, regex))
		{
			for (std::smatch::iterator iter = matches.cbegin(); iter != matches.cend(); iter++)
			{
				regex_match match;
				match.content = iter->str();
				match.index = std::distance(input.cbegin(), iter->first);
				match.length = iter->length();
				res.emplace_back(match);
			}
			start = matches.suffix().first;
		}

		return res;
	}

	static inline std::vector<std::string> split_string(const std::string& input, const std::regex& regex, bool no_empty_strings = true)
	{
		std::vector<std::string> res;

		// find possible split-spots
		std::vector<regex_match> matches = get_matches(input, regex);
		if (matches.empty())
			return res;

		// get regions between split-spots
		for (uint64_t i = 0; i < matches.size(); i++)
		{
			if (!i)
			{
				if (matches[i].index) // account for case that 'regex' is at the front
				{
					res.emplace_back(input.substr(0, matches[i].index));
				}
			}
			else
			{
				res.emplace_back(input.substr(matches[i - 1].index + matches[i - 1].length, matches[i].index - (matches[i - 1].index + matches[i - 1].length)));
			}
		}
		if (matches.back().index + matches.back().length < input.length()) // account for case that 'regex' is at the end
		{
			res.emplace_back(input.substr(matches.back().index + matches.back().length, input.length() - matches.back().index));
		}

		// clear possible empty strings
		if (no_empty_strings)
		{
			clear_empty_strings(res);
		}

		return res;
	}

}
////////////////////////////////////////
#endif
////////////////////////////////////////