////////////////////////////////////////
/// general utility header-only-library
/// for convenience methods/types in C++
/// 2025 Julian Benzel
////////////////////////////////////////
/// file i/o and manipulation
////////////////////////////////////////
#ifndef UTILITYLIB_IO_HPP
#define UTILITYLIB_IO_HPP
////////////////////////////////////////
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
////////////////////////////////////////
namespace util
{
	typedef std::vector<uint8_t> bytebuffer;

	static inline std::string to_string(const bytebuffer& buffer)
	{
		std::string res;
		for (size_t i = 0; i < buffer.size(); i++)
		{
			res.append(1, buffer[i]);
		}
		return res;
	}

	static inline bytebuffer to_buffer(const std::string& str)
	{
		bytebuffer res;
		for (size_t i = 0; i < str.size(); i++)
		{
			res.emplace_back(static_cast<uint8_t>(str[i]));
		}
		return res;
	}

	template <typename Type>
	std::vector<Type> subvector(const std::vector<Type>& vec, size_t start_index, size_t end_index)
	{
		std::vector<Type> res;
		if (start_index >= vec.size() || end_index >= vec.size())
		{
			throw std::runtime_error("index out of range");
		}
		if (end_index < start_index)
		{
			throw std::runtime_error("end-index is greater than start-index");
		}
		for (size_t i = start_index; i < end_index; i++)
		{
			res.emplace_back(vec[i]);
		}
		return res;
	}

	static inline bool readfile(const std::filesystem::path& path, bytebuffer& contents_out)
	{
		try
		{
			std::ifstream file_reader(path, std::ifstream::binary);
			if (file_reader.bad())
			{
				throw std::runtime_error("file not found");
			}
			size_t file_size = std::filesystem::file_size(path);
			for (size_t i = 0; i < file_size; i++)
			{
				contents_out.emplace_back(static_cast<uint8_t>(file_reader.get()));
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

	static inline bool writetofile(const std::filesystem::path& path, const bytebuffer& contents)
	{
		try
		{
			std::ofstream file_writer(path, std::ofstream::app | std::ofstream::binary);
			if (file_writer.bad())
			{
				throw std::runtime_error("file not found");
			}
			for (size_t i = 0; i < contents.size(); i++)
			{
				file_writer.put(contents[i]);
			}
			file_writer.close();
		}
		catch (const std::exception& e)
		{
			std::cerr << "exception thrown while trying to write to file '" << path.string() << "': " << e.what() << std::endl;
			return false;
		}
		return true;
	}

	static inline bool overwritefile(const std::filesystem::path& path, const bytebuffer& contents)
	{
		try
		{
			std::ofstream file_writer(path, std::ofstream::binary);
			if (file_writer.bad())
			{
				throw std::runtime_error("file not found");
			}
			for (size_t i = 0; i < contents.size(); i++)
			{
				file_writer.put(contents[i]);
			}
			file_writer.close();
		}
		catch (const std::exception& e)
		{
			std::cerr << "exception thrown while trying to overwrite file '" << path.string() << "': " << e.what() << std::endl;
			return false;
		}
		return true;
	}

	static inline bool insertinfile(const std::filesystem::path& path, const bytebuffer& contents, uint64_t start_index = 0u)
	{
		try
		{
			bytebuffer fcontents;
			readfile(path, fcontents);
			if (start_index >= fcontents.size())
			{
				throw std::runtime_error("index out of range");
			}

			bytebuffer before_insert = subvector(fcontents, 0, start_index);
			bytebuffer after_insert = subvector(fcontents, start_index, fcontents.size() - 1);

			std::ofstream file_writer(path, std::ofstream::binary);
			if (file_writer.bad())
			{
				throw std::runtime_error("file not found");
			}
			for (size_t i = 0; i < before_insert.size(); i++)
			{
				file_writer.put(before_insert[i]);
			}
			for (size_t i = 0; i < contents.size(); i++)
			{
				file_writer.put(contents[i]);
			}
			for (size_t i = 0; i < after_insert.size(); i++)
			{
				file_writer.put(after_insert[i]);
			}
			file_writer.close();
		}
		catch (const std::exception& e)
		{
			std::cerr << "exception thrown while trying to insert to file '" << path.string() << "': " << e.what() << std::endl;
			return false;
		}
		return true;
	}
}
////////////////////////////////////////
#endif
////////////////////////////////////////