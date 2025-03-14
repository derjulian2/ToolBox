////////////////////////////////////////
/// general utility header-only-library
/// for convenience methods/types in C++
/// 2025 Julian Benzel
////////////////////////////////////////
/// string operation utility
////////////////////////////////////////
#ifndef UTILITYLIB_STRINGMANIP_HPP
#define UTILITYLIB_STRINGMANIP_HPP
////////////////////////////////////////
#include <vector>
#include <string>
#include <concepts>
#include <iostream>
////////////////////////////////////////
namespace util
{
	template <typename Type>
	concept container_c = requires(const Type & t)
	{
		typename Type::const_iterator;
		t.cbegin();
		t.cend();
		t.size();
	};
	
	template <typename T>
	concept printable_c = requires(const T & elem)
	{
		std::cout << elem;
	};

/*
* print containers that satisfy the constraints
*/

	template <template <typename> typename _container_t, typename _elem_t>
	requires container_c<_container_t<_elem_t>> and printable_c<_elem_t>
	constexpr static inline std::ostream& operator<<(std::ostream& os, const _container_t<_elem_t>& range)
	{
		if (!range.size())
		{
			os << "[ ]";
			return os;
		}
	
		typename _container_t<_elem_t>::const_iterator  begin = range.cbegin();
		typename _container_t<_elem_t>::const_iterator  end = range.cend();
		typename _container_t<_elem_t>::const_iterator  last_element = range.cend();
		last_element--;
	
		os << "[ ";
		for (typename _container_t<_elem_t>::const_iterator i = begin; i != end; i++)
		{
			os << *i;
			if (i != last_element)
			{
				os << ", ";
			}
			else
			{
				os << " ";
			}
		}
		os << "]";
		return os;
	}

	template <template <class, class> class _container_t, class _elem_t, template <class> class _allocator_t>
	requires container_c<_container_t<_elem_t, _allocator_t<_elem_t>>> and printable_c<_elem_t>
	constexpr static inline std::ostream& operator<<(std::ostream& os, const _container_t<_elem_t, _allocator_t<_elem_t>>& range)
	{
		if (!range.size())
		{
			os << "[ ]";
			return os;
		}
	
		typename _container_t<_elem_t, _allocator_t<_elem_t>>::const_iterator  begin = range.cbegin();
		typename _container_t<_elem_t, _allocator_t<_elem_t>>::const_iterator  end   = range.cend();
		typename _container_t<_elem_t, _allocator_t<_elem_t>>::const_iterator  last_element = range.cend();
		last_element--;
	
		os << "[ ";
		for (typename _container_t<_elem_t, _allocator_t<_elem_t>>::const_iterator i = begin; i != end; i++)
		{
			os << *i;
			if (i != last_element)
			{
				os << ", ";
			}
			else
			{
				os << " ";
			}
		}
		os << "]";
		return os;
	}
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

	static inline std::vector<std::string> split_string(const std::string& input, const std::string& delimiter, bool no_empty_strings = true)
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
}
////////////////////////////////////////
#endif
////////////////////////////////////////
