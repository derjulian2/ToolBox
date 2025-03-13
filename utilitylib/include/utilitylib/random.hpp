////////////////////////////////////////
/// general utility header-only-library
/// for convenience methods/types in C++
/// 2025 Julian Benzel
////////////////////////////////////////
/// random number generation
////////////////////////////////////////
#ifndef UTILITYLIB_RANDOM_HPP
#define UTILITYLIB_RANDOM_HPP
////////////////////////////////////////
#include <cstdint>
#include <stdexcept>
#include <random>
#include <vector>
////////////////////////////////////////
namespace util
{
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
}
////////////////////////////////////////
#endif
////////////////////////////////////////