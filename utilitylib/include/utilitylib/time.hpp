////////////////////////////////////////
/// general utility header-only-library
/// for convenience methods/types in C++
/// 2025 Julian Benzel
////////////////////////////////////////
/// date and time utility
////////////////////////////////////////
#ifndef UTILITYLIB_TIME_HPP
#define UTILITYLIB_TIME_HPP
////////////////////////////////////////
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <functional>
////////////////////////////////////////
namespace util
{
	class timestamp
	{
	public:

		static inline std::string default_format = "%d.%m.%Y %H:%M:%S";

		inline timestamp(const std::string& format = default_format)
		{
			time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
			setformat(format);
		}

		inline timestamp(const std::chrono::milliseconds& time_since_epoch, const std::string& format = default_format)
		{
			time_point = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>{ time_since_epoch };
			setformat(format);
		}

		void setformat(const std::string& format)
		{
			this->format = format;
			update_timestamp_str();
		}

		inline std::chrono::milliseconds milliseconds_since_epoch() const
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch());
		}

		inline std::string to_string() const
		{
			return this->timestamp_str;
		}

		inline operator std::string()
		{
			return this->timestamp_str;
		}

		static inline std::string get()
		{
			return timestamp().timestamp_str;
		}

		template <typename Rep, typename Period>
		timestamp operator+(std::chrono::duration<Rep, Period> p)
		{
			timestamp res(*this);
			res.time_point += p;
			res.update_timestamp_str();
			return res;
		}

		template <typename Rep, typename Period>
		timestamp operator-(std::chrono::duration<Rep, Period> p)
		{
			timestamp res(*this);
			res.time_point -= p;
			res.update_timestamp_str();
			return res;
		}

		template <typename Rep, typename Period>
		timestamp& operator+=(std::chrono::duration<Rep, Period> p)
		{
			this->time_point += p;
			update_timestamp_str();
			return *this;
		}

		template <typename Rep, typename Period>
		timestamp& operator-=(std::chrono::duration<Rep, Period> p)
		{
			this->time_point -= p;
			update_timestamp_str();
			return *this;
		}

		timestamp operator+(const timestamp& t)
		{
			timestamp res(*this);
			res.time_point += t.time_point.time_since_epoch();
			res.update_timestamp_str();
			return res;
		}

		timestamp operator-(const timestamp& t)
		{
			timestamp res(*this);
			res.time_point -= t.time_point.time_since_epoch();
			res.update_timestamp_str();
			return res;
		}

	private:

		std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> time_point;
		std::string format;
		std::string timestamp_str;

		void update_timestamp_str()
		{
			std::time_t _time_t = std::chrono::system_clock::to_time_t(time_point);
			std::tm _tm = std::tm();
			gmtime_s(&_tm, &_time_t);
			
			std::stringstream sstream;
			sstream << std::put_time(&_tm, format.c_str());
			timestamp_str = sstream.str();
		}
	};

	static inline std::ostream& operator<<(std::ostream& os, const timestamp& ts)
	{
		os << ts.to_string();
		return os;
	}

	/*
	* [blocking]
	* executes the passed function as often as it can for the specified time frame.
	* amount of times the function gets executed may vary and strongly depend on resource allocation
	* 
	* to do: should count time in another thread and cancel if timeout is reached or sth
	* somehow blocking functions need to be canceled after timeout
	*/
	template <typename Rep, typename Period>
	static inline void execute_for_time(std::function<void(bool& quit)> func, std::chrono::duration<Rep, Period> time_frame)
	{
		timestamp current_time;
		timestamp end_time = current_time.milliseconds_since_epoch() + std::chrono::duration_cast<std::chrono::milliseconds>(time_frame);
		bool quit = false;
		while (current_time.milliseconds_since_epoch() < end_time.milliseconds_since_epoch() && !quit)
		{
			func(quit);
			current_time = timestamp();
		}
	}
}
////////////////////////////////////////
#endif
////////////////////////////////////////