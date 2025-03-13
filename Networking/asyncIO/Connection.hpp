//////////////////////////////////////////////////
#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP
//////////////////////////////////////////////////
#include <memory>
#include <iostream>
//////////////////////////////////////////////////
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
//////////////////////////////////////////////////
#define UTIL_MOD_TIMESTAMP
#define UTIL_MOD_STRINGMANIP
#define UTIL_MOD_FILEMANIP
#include "../utilitylib.hpp"
//////////////////////////////////////////////////
namespace tcp
{
	//////////////////////////////////////////////////

	class Logging
	{
	public:
		void Log(const std::string&);
		std::string getLog() const;
		std::string getLastLog() const;
	protected:
		std::vector<std::string> log;
	};

	//////////////////////////////////////////////////

	class Connection : public Logging, public boost::noncopyable
	{
	private:
		std::shared_ptr<boost::asio::io_context> io_context = nullptr;
		std::shared_ptr<boost::asio::ip::tcp::socket> socket = nullptr;
		bool VALID		= false;
		bool SENDING	= false;
		bool RECEIVING	= false;
	public:
		Connection(std::shared_ptr<boost::asio::io_context> ioc);
		~Connection();
		void close();

		bool valid() const;

		template <typename Rep, typename Period>
		bool send(const std::string& msg,
			const std::function<void(size_t)>& complete_callback,
			std::chrono::duration<Rep, Period> timeout);
		
		template <typename Rep, typename Period>
		bool receive(std::string& out,
			const std::function<void(size_t)>& complete_callback,
			size_t transfer_at_least,
			std::chrono::duration<Rep, Period> timeout);
	private:
		friend class Server;
		friend class Client;
	};

	//////////////////////////////////////////////////

	template <typename Rep, typename Period>
	bool tcp::Connection::send(const std::string& msg,
		const std::function<void(size_t)>& complete_callback,
		std::chrono::duration<Rep, Period> timeout)
	{
		if (!VALID)
		{
			return false;
		}
		SENDING = true;
		boost::asio::streambuf buffer;
		buffer.prepare(512U);
		buffer.sputn(msg.c_str(), msg.size());
		boost::asio::async_write(*this->socket, buffer,
			[&, complete_callback](const boost::system::error_code& err, size_t bytes)
			{
				if (err)
				{
					close();
					Log("connection closed because an error occured: " + err.message());
				}
				else
				{
					Log("successfully sent " + std::to_string(bytes) + " bytes");
					if (complete_callback)
					{
						complete_callback(bytes);
					}
				}
				SENDING = false;
			});
		if (io_context->stopped())
		{
			io_context->restart();
		}
		if (timeout.count())
		{
			if (io_context->run_one_for(timeout))
			{
				return true;
			}
			return false;
		}
		else
		{
			if (io_context->run_one())
			{
				return true;
			}
			return false;
		}
	}

	template <typename Rep, typename Period>
	bool tcp::Connection::receive(std::string& out,
		const std::function<void(size_t)>& complete_callback,
		size_t transfer_at_least,
		std::chrono::duration<Rep, Period> timeout)
	{
		if (!VALID)
		{
			return false;
		}
		RECEIVING = true;
		boost::asio::streambuf buffer;
		buffer.prepare(512U);
		boost::asio::async_read(*this->socket, buffer, boost::asio::transfer_at_least(transfer_at_least),
			[&, complete_callback](const boost::system::error_code& err, size_t bytes)
			{
				if (err)
				{
					close();
					Log("connection closed because an error occured: " + err.message());
				}
				else
				{
					std::istream istream(&buffer);
					out.clear();
					for (size_t i = 0; i < bytes; i++) 
					{
						out.append(1, static_cast<unsigned char>(istream.get()));
					}
					Log("successfully received " + std::to_string(bytes) + " bytes");
					if (complete_callback)
					{
						complete_callback(bytes);
					}
				}
				RECEIVING = false;
			});
		if (io_context->stopped())
		{
			io_context->restart();
		}
		if (timeout.count())
		{
			if (io_context->run_one_for(timeout))
			{
				return true;
			}
			return false;
		}
		else
		{
			if (io_context->run_one())
			{
				return true;
			}
			return false;
		}
	}
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////