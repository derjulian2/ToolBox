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
    /*
     * prime testing function
     * @param _num - unsigned integer type to test
     * @tparam any type that satisfies std::unsigned_integral
     * @returns true if _num is prime, otherwise false
     */
    template<typename num>
        requires std::unsigned_integral<num>
    static inline constexpr bool is_prime(const num& _num)
    {
        if (_num == 2U) { return true; }
        num _iter = 1U;
        do { if (!(_num % ++_iter)) { return false; }; } 
        while (_iter <= static_cast<num>(std::floor(std::sqrt(static_cast<double>(_num)))));
        return true;
    }
	/*
	* random unsigned integer generation
	* @param lower - the smallest possible random number
	* @param upper - the largest possible random number
	* @returns a random number equal to or between 'lower' and 'upper'
	* @throws invalid_argument if upper < lower
	*/
	static inline uint64_t randint(uint64_t lower, uint64_t upper)
	{
		if (upper < lower)
		{
			throw std::invalid_argument("upper range cannot be smaller than lower range");
		}
		if (upper == lower)
		{
			return upper;
		}

		std::random_device random_device;
		std::mt19937 random_number_generator(random_device());
		std::uniform_int_distribution<std::mt19937::result_type> distribution(lower, upper);

		return static_cast<uint64_t>(distribution(random_number_generator));
	}
	/*
	* generates vector of random unique unsigned integers
	* @param lower - the smallest possible random number
	* @param upper - the largest possible random number
	* @param size  - the amount of random numbers / the size of the vector
	* @returns vector of random numbers possibly including 'lower' or 'upper'
	* @throws invalid_argument if upper < lower
	* @throws invalid_argument if size is greater than the amount of numbers between 'lower' and 'upper'
	*/
	static inline std::vector<uint64_t> random_uniques(uint64_t lower, uint64_t upper, uint64_t size)
	{
		if (upper < lower)
		{
			throw std::invalid_argument("upper range cannot be smaller that lower range");
		}
		if (size - 1 > upper - lower)
		{
			throw std::invalid_argument("not enough numbers in specified range to fulfill size requirements");
		}

		std::vector<uint64_t> res;
		std::vector<uint64_t> numbers;

		for (uint64_t i = lower; i <= upper; i++)
		{
			numbers.emplace_back(i);
		}
		
		std::random_device random_device;
		std::mt19937 random_number_generator(random_device());

		for (uint64_t i = 0; i < size; i++)
		{
			std::uniform_int_distribution<std::mt19937::result_type> distribution(0, numbers.size() - 1);
			std::vector<uint64_t>::iterator next_num = numbers.begin() + static_cast<uint64_t>(distribution(random_number_generator));
			res.emplace_back(*next_num);
			numbers.erase(next_num);
		}
		
		return res;
	}
}
////////////////////////////////////////
#endif
////////////////////////////////////////
