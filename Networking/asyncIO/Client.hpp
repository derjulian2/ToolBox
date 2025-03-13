//////////////////////////////////////////////////
#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP
//////////////////////////////////////////////////
#include <memory>
//////////////////////////////////////////////////
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
//////////////////////////////////////////////////
#include "Connection.hpp"
//////////////////////////////////////////////////
namespace tcp
{
	constexpr size_t PACKET_SIZE = 512u;

	//////////////////////////////////////////////////

	class Client : public boost::noncopyable, public Logging
	{
	private:
		std::shared_ptr<boost::asio::io_context> io_context = nullptr;
		std::shared_ptr<boost::asio::ip::tcp::resolver> resolver = nullptr;
		std::shared_ptr<Connection> connection = nullptr;
		std::string ip;
		std::string port;
		bool VALID = false;
	public:
		Client(const std::string& ip, const std::string& port);
		~Client();
		void close();
		void run();
		
		bool valid() const;

		template <typename Rep, typename Period>
		bool connect(const std::function<void(void)>& complete_callback,
			std::chrono::duration<Rep, Period> timeout);

		bool ping();
		bool sendfile(const std::filesystem::path& path);

		Connection& getConnection();
	private:

	};

	//////////////////////////////////////////////////

	template <typename Rep, typename Period>
	bool tcp::Client::connect(const std::function<void(void)>& complete_callback,
		std::chrono::duration<Rep, Period> timeout)
	{
		if (!VALID)
		{
			return false;
		}
		resolver->async_resolve(ip, port,
			[&, complete_callback](const boost::system::error_code& err, const boost::asio::ip::tcp::resolver::results_type& results)
			{
				if (err)
				{
					close();
					Log("error in establishing connection with server: " + err.message());
				}
				else
				{
					boost::asio::async_connect(*this->connection->socket, results,
						[&, complete_callback](const boost::system::error_code& _err, const boost::asio::ip::tcp::endpoint& ep)
						{
							if (_err)
							{
								close();
								Log("error in establishing connection with server: " + _err.message());
							}
							else
							{
								Log("successfully established connection with server: at " + ep.address().to_string() + " on port " + std::to_string(ep.port()));
								if (complete_callback)
								{
									complete_callback();
								}
							}
						});
					if (timeout.count())
					{
						io_context->run_one_for(timeout);
					}
					else
					{
						io_context->run_one();
					}
				}
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