////////////////////////////////////////
/// general utility library
/// for convenience methods in C++
////////////////////////////////////////
/// this single-header library is divided into modules
/// define the corresponding macros
/// before including to activate the respective module
/// 
/// all modules:					#define UTIL_MOD_ALL
/// - random						#define UTIL_MOD_RANDOM
/// - timestamp						#define UTIL_MOD_TIMESTAMP
/// - string operations				#define UTIL_MOD_STRINGMANIP
/// - regex operations				#define UTIL_MOD_REGEX
/// - file operations				#define UTIL_MOD_FILEMANIP
////////////////////////////////////////
#ifndef UTIL_H
#define UTIL_H
////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#if defined(UTIL_MOD_RANDOM) || defined(UTIL_MOD_ALL)
#include <random>
#include <cstdint>
#include <vector>
#endif
#if defined(UTIL_MOD_TIMESTAMP) || defined(UTIL_MOD_ALL)
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#endif
#if defined(UTIL_MOD_STRINGMANIP) || defined(UTIL_MOD_ALL)
#include <iostream>
#include <string>
#include <concepts>
#endif
#if defined(UTIL_MOD_REGEX) || defined(UTIL_MOD_ALL)
#include <iostream>
#include <string>
#include <regex>
#include <concepts>
#endif
#if defined(UTIL_MOD_FILEMANIP) || defined(UTIL_MOD_ALL)
#include <iostream>
#include <filesystem>
#include <fstream>
#endif
////////////////////////////////////////
namespace utility
{

	////////////////////////////////////////
	/// random number generation
	////////////////////////////////////////

#if defined(UTIL_MOD_RANDOM) || defined(UTIL_MOD_ALL)

	static inline uint64_t randint(uint64_t lower_range, uint64_t upper_range)
	{
		if (upper_range < lower_range)
			throw std::invalid_argument("randint: upper range cannot be smaller than lower range");
		std::random_device random_device;
		std::mt19937 random_number_generator(random_device());
		std::uniform_int_distribution<std::mt19937::result_type> distribution(lower_range, upper_range);

		return static_cast<uint64_t>(distribution(random_number_generator));
	}

	static inline std::vector<uint64_t> random_uniques(uint64_t lower_range, uint64_t upper_range, uint64_t size)
	{
		std::vector<uint64_t> res;
		
		uint64_t unique_counter = 0;
		while (unique_counter != size)
		{
			uint64_t nextint = randint(lower_range, upper_range);
			bool is_unique = true;
			for (uint64_t i = 0; i < res.size(); i++)
			{
				if (nextint == res[i])
				{
					is_unique = false;
					break;
				}
			}
			if (is_unique)
			{
				res.emplace_back(nextint);
				unique_counter++;
			}
		}
		return res;
	}

#endif

	////////////////////////////////////////
	/// convenience timestamp
	/// [[remove gmtime() because deprecated]]
	////////////////////////////////////////

#if defined(UTIL_MOD_TIMESTAMP) || defined(UTIL_MOD_ALL)

	struct Timestamp
	{
		inline Timestamp()
		{
			std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
			count = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
			std::time_t time_t = std::chrono::system_clock::to_time_t(tp);

			std::stringstream datebuffer;
			datebuffer << std::put_time(std::gmtime(&time_t), "%d.%m.%Y");
			date = datebuffer.str();

			std::stringstream timebuffer;
			timebuffer << std::put_time(std::gmtime(&time_t), "%H:%M:%S");
			time = timebuffer.str();

			timestamp = date + " " + time;
		}

		inline Timestamp(const uint64_t& time_since_epoch)
		{
			std::chrono::system_clock::time_point tp{ std::chrono::milliseconds{time_since_epoch} }; // initializer braces very useful here
			count = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
			std::time_t time_t = std::chrono::system_clock::to_time_t(tp);

			std::stringstream datebuffer;
			datebuffer << std::put_time(std::gmtime(&time_t), "%d.%m.%Y");
			date = datebuffer.str();

			std::stringstream timebuffer;
			timebuffer << std::put_time(std::gmtime(&time_t), "%H:%M:%S");
			time = timebuffer.str();

			timestamp = date + " " + time;
		}

		static inline std::string get()
		{
			return Timestamp().timestamp;
		}

		std::string date;
		std::string time;
		std::string timestamp;
		uint64_t count;
	};

#endif

	////////////////////////////////////////
	/// to_string utility
	////////////////////////////////////////

#if defined(UTIL_MOD_STRINGMANIP) || defined(UTIL_MOD_REGEX) || defined(UTIL_MOD_ALL)

	template <typename T>
	concept Printable = requires(const T & elem)
	{
		std::cout << elem;
	};

	template <Printable T>
	static inline std::string to_string(const std::vector<T>& vec)
	{
		std::stringstream res;
		res << "[ ";
		for (size_t iter = 0; iter < vec.size(); iter++)
		{
			res << vec[iter];
			if (iter != vec.size() - 1)
				res << ", ";
		}
		res << " ]";
		return res.str();
	}

#endif

	////////////////////////////////////////
	/// string operations
	/// [[TEST FOR UNDEFINED BEHAVIOUR]]
	////////////////////////////////////////

#if defined(UTIL_MOD_STRINGMANIP) || defined(UTIL_MOD_REGEX) || defined(UTIL_MOD_ALL)

	/*
	* considers a string empty if it does not contain any alphanumerical values or special characters
	* (e.g. only whitespace or special ascii characters)
	*/
	static inline bool isempty(const std::string& str)
	{
		for (char c : str)
		{
			if (c >= 33 && c <= 126)
			{
				return false;
			}
		}
		return true;
	}

	static inline void clear_empty_strings(std::vector<std::string>& vec)
	{
		for (std::vector<std::string>::iterator iter = vec.begin(); iter != vec.end();)
		{
			if (isempty(*iter))
			{
				iter = vec.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	static inline std::string tolowercase(const std::string& str)
	{
		std::string res;
		for (char c : str)
		{
			res.append(1, std::tolower(c));
		}
		return res;
	}

	static inline std::string touppercase(const std::string& str)
	{
		std::string res;
		for (char c : str)
		{
			res.append(1, std::toupper(c));
		}
		return res;
	}

	static inline std::vector<std::string> split_string(const std::string& input, const std::string& split, bool no_empty_strings = true)
	{
		std::vector<std::string> res;
		size_t index = 0;

		// find possible split-spots
		std::vector<size_t> indices;
		while ((index = input.find(split, index)) != std::string::npos)
		{
			indices.emplace_back(index);
			index += split.length();
		}
		if (!indices.size())
		{
			return res;
		}

		// get regions between split-spots
		for (uint64_t i = 0; i < indices.size(); i++)
		{
			if (!i)
			{
				if (indices[i]) // account for case that 'split' is at the front
				{
					res.emplace_back(input.substr(0, indices[i]));
				}
			}
			else
			{
				res.emplace_back(input.substr(indices[i - 1] + split.length(), indices[i] - (indices[i - 1] + split.length())));
			}
		}
		if (indices.back() + split.length() < input.length()) // account for case that 'split' is at the end
		{
			res.emplace_back(input.substr(indices.back() + split.length(), input.length() - indices.back()));
		}

		// clear possible empty strings
		if (no_empty_strings)
		{
			clear_empty_strings(res);
		}
		return res;
	}

#endif

	////////////////////////////////////////
	/// regex operations
	/// [[TEST FOR UNDEFINED BEHAVIOUR]]
	////////////////////////////////////////

#if defined(UTIL_MOD_REGEX) || defined(UTIL_MOD_ALL)

	struct RegexMatch
	{
		std::string content;
		size_t length;
		size_t index;
	};

	static inline std::vector<RegexMatch> get_matches(const std::string& input, const std::regex& regex)
	{
		std::vector<RegexMatch> res;
		std::smatch matches;

		std::string::const_iterator start = input.cbegin();
		while (std::regex_search(start, input.cend(), matches, regex))
		{
			for (std::smatch::iterator iter = matches.cbegin(); iter != matches.cend(); iter++)
			{
				RegexMatch match;
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
		std::vector<RegexMatch> matches = get_matches(input, regex);
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

#endif 

	////////////////////////////////////////
	/// file input/output and manipulation
	////////////////////////////////////////

#if defined(UTIL_MOD_FILEMANIP) || defined(UTIL_MOD_ALL)

	static inline bool readfile(const std::filesystem::path& path, std::string& contents_out)
	{
		try
		{
			std::ifstream file_reader(path);
			if (file_reader.bad())
				throw std::runtime_error("file not found");
			int token;
			while ((token = file_reader.get()) != std::ifstream::traits_type::eof())
			{
				contents_out.append(1, token);
			}
			file_reader.close();
		}
		catch (const std::exception& e)
		{
			std::cerr << "exception thrown while trying to read file '" << path.string() << "': " << e.what() << std::endl;
			return false;
		}
		return true;
	}

	static inline bool writetofile(const std::filesystem::path& path, const std::string& contents)
	{
		try
		{
			std::ofstream file_writer(path, std::ofstream::app);
			if (file_writer.bad())
				throw std::runtime_error("file not found");
			file_writer.write(contents.c_str(), contents.size());
			file_writer.close();
		}
		catch (const std::exception& e)
		{
			std::cerr << "exception thrown while trying to write to file '" << path.string() << "': " << e.what() << std::endl;
			return false;
		}
		return true;
	}

	static inline bool overwritefile(const std::filesystem::path& path, const std::string& contents)
	{
		try
		{
			std::ofstream file_writer(path);
			if (file_writer.bad())
				throw std::runtime_error("file not found");
			file_writer.write(contents.c_str(), contents.size());
			file_writer.close();
		}
		catch (const std::exception& e)
		{
			std::cerr << "exception thrown while trying to overwrite file '" << path.string() << "': " << e.what() << std::endl;
			return false;
		}
		return true;
	}

	static inline bool insertinfile(const std::filesystem::path& path, const std::string& contents, uint64_t start_index = 0)
	{
		try
		{
			std::string fcontents;
			readfile(path, fcontents);
			if (start_index >= fcontents.size())
				throw std::runtime_error("index out of range");
			overwritefile(path, fcontents.substr(0, start_index));
			writetofile(path, contents + fcontents.substr(start_index));
		}
		catch (const std::exception& e)
		{
			std::cerr << "exception thrown while trying to insert to file '" << path.string() << "': " << e.what() << std::endl;
			return false;
		}
		return true;
	}

#endif

}
////////////////////////////////////////
#endif
////////////////////////////////////////