////////////////////////////////////////
/// general utility library
/// for convenience methods in C++
////////////////////////////////////////
/// divided into modules
/// define the corresponding macros
/// before including to activate
/// 
/// all modules:					#define UTIL_MOD_ALL
/// - random						#define UTIL_MOD_RANDOM
/// - timestamp						#define UTIL_MOD_TIMESTAMP
/// - regex and string operations	#define UTIL_MOD_STRINGMANIP
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
	/// convenient timestamp
	////////////////////////////////////////
#if defined(UTIL_MOD_TIMESTAMP) || defined(UTIL_MOD_ALL)

	struct Timestamp
	{
		inline Timestamp()
		{
			const std::chrono::time_point tp = std::chrono::system_clock::now();
			count = tp.time_since_epoch().count();
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
	/// regex and string operations
	/// [[TEST FOR UNDEFINED BEHAVIOUR]]
	////////////////////////////////////////
#if defined(UTIL_MOD_STRINGMANIP) || defined(UTIL_MOD_ALL)

	struct RegexMatch
	{
		std::string content;
		std::string::const_iterator iter;
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
			for (std::smatch::const_iterator it = matches.cbegin(); it != matches.cend(); it++)
			{
				RegexMatch match = {
					.content = *it,
					.iter = matches.suffix().first,
					.length = static_cast<size_t>((*it).length()),
					.index = static_cast<size_t>(std::distance(input.cbegin(), matches.suffix().first))
				};
				res.emplace_back(match);
			}
			start = matches.suffix().first;
		}

		return res;
	}

	static inline std::vector<std::string> split_string(const std::string& input, const std::regex& regex)
	{
		std::vector<std::string> res;

		std::vector<RegexMatch> matches = get_matches(input, regex);
		RegexMatch last_match = { "", {}, NULL, NULL };

		if (matches.empty())
			return res;

		for (size_t i = 0; i < matches.size(); i++)
		{
			if (matches[i].index != last_match.index + last_match.length)
				res.emplace_back(input.substr(i == 0 ? 0 : last_match.index + last_match.length - 1,  matches[i].index - last_match.index - 1));
			last_match = matches[i];
		}
		res.emplace_back(input.substr(last_match.index + last_match.length - 1, input.npos));
		return res;
	}

	static inline std::vector<std::string> split_string(const std::string& input, const std::string& regex)
	{
		return split_string(input, std::regex(regex));
	}

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