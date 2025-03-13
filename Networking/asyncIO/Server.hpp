//////////////////////////////////////////////////
#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP
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
	//////////////////////////////////////////////////

	class Server : public boost::noncopyable, public Logging
	{
	private:
		std::shared_ptr<boost::asio::io_context> io_context = nullptr;
		std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor = nullptr;
		std::shared_ptr<Connection> connection = nullptr;
		bool VALID = false;
	public:
		Server(const std::string& port = "0");
		~Server();
		void close();
		void run();

		bool valid() const;

		template <typename Rep, typename Period>
		bool accept(const std::function<void(void)>& complete_callback,
			std::chrono::duration<Rep, Period> timeout);

		bool receiveping();
		bool receivefile(const std::filesystem::path& write_path);

		Connection& getConnection();
	private:

	};

	//////////////////////////////////////////////////

	template <typename Rep, typename Period>
	bool tcp::Server::accept(const std::function<void(void)>& complete_callback,
		std::chrono::duration<Rep, Period> timeout)
	{
		if (!VALID)
		{
			return false;
		}
		acceptor->async_accept(*this->connection->socket,
			[&, complete_callback](const boost::system::error_code& err)
			{
				if (err)
				{
					close();
					Log("connection closed because an error occured: " + err.message());
				}
				else
				{
					Log("successfully accepted new client");
					if (complete_callback)
					{
						complete_callback();
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