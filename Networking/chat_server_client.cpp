
#define IP boost::asio::ip::address_v4::loopback()
#define PORT 55555
#define PORT_STR "55555"

//#define BUILD_SERVER
#define BUILD_CLIENT

#include <thread>
#include <boost/asio.hpp>
#include <iostream>

constexpr boost::asio::chrono::duration receive_interval = boost::asio::chrono::seconds(3);

boost::asio::streambuf send_buf;
boost::asio::streambuf recv_buf;

boost::asio::io_context context;
boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard = boost::asio::make_work_guard(context);
boost::asio::strand<boost::asio::io_context::executor_type> strand = boost::asio::make_strand(context);

static inline void buffer_string(boost::asio::streambuf& _buf, const std::string& _data)
{
	_buf.prepare(_data.size() + 1);
	_buf.sputn(_data.c_str(), _data.size() + 1);
}

static inline void extract_string(boost::asio::streambuf& streambuf, std::string& _out)
{
	_out = std::string(static_cast<const char*>(streambuf.data().data()));
	streambuf.consume(streambuf.size());
}

static inline void receive_handler(boost::asio::ip::tcp::socket& _socket, const boost::system::error_code& err, const std::size_t& bytes_transferred)
{
	if (!err)
	{
		if (bytes_transferred)
		{
			std::string out;
			extract_string(recv_buf, out);
			std::cout << bytes_transferred << " byte msg: " << out << std::endl;
		}
		boost::asio::async_read(_socket, // see completion handler immediatly posts new async_read operation to the io_context to keep reading indefinitely
			recv_buf,
			boost::asio::transfer_at_least(1U),
			boost::asio::bind_executor(strand, std::bind(&receive_handler,
				std::ref(_socket),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
	}
	else
	{
		std::cerr << "server error with receive_handler: " << err.message() << std::endl;
	}
}

static inline void send_handler(const boost::system::error_code& err, const std::size_t& bytes_transferred)
{
	if (!err)
	{
		std::cout << bytes_transferred << " byte msg sent" << std::endl;
	}
	else
	{
		std::cerr << "server error with send_handler: " << err.message() << std::endl;
	}
}

#ifdef BUILD_SERVER

static inline void server_handle_accept(boost::asio::ip::tcp::socket& _socket, const boost::system::error_code& err)
{
	if (!err)
	{
		boost::asio::async_read(_socket,
			recv_buf,
			boost::asio::transfer_at_least(1U),
			boost::asio::bind_executor(strand, std::bind(&receive_handler,
				std::ref(_socket),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
		std::cout << std::endl << "server successfully accepted new client" << std::endl;
	}
	else
	{
		std::cerr << "server error with async_accept: " << err.message() << std::endl;
	}
}

#elif defined(BUILD_CLIENT)

static inline void client_handle_connect(boost::asio::ip::tcp::socket& _socket, const boost::system::error_code& err)
{
	if (!err)
	{
		boost::asio::async_read(_socket,
			recv_buf,
			boost::asio::transfer_at_least(1U),
			boost::asio::bind_executor(strand, std::bind(&receive_handler,
				std::ref(_socket),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
		std::cout << std::endl << "client successfully connected to server" << std::endl;
	}
	else
	{
		std::cerr << "client error with async_connect: " << err.message() << std::endl;
	}
}

#endif
/*
 * simple asynchronous, multithreaded chat server/client application / minimal working example
 * 
 * concept:
 * - dialog / gui makes asynchronous requests to the socket, networking thread executes them to achieve high responsiveness
 */
int main(int argc, char** argv)
{
	try
	{
		std::thread network_thread(&boost::asio::io_context::run, &context);

		std::string in;
		bool dialog = true;
		
#ifdef BUILD_SERVER

		boost::asio::ip::tcp::socket server_socket(context);
		boost::asio::ip::tcp::acceptor acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT));
		
		acceptor.async_accept(server_socket, boost::asio::bind_executor(strand, std::bind(&server_handle_accept, std::ref(server_socket), boost::asio::placeholders::error)));

		while (dialog)
		{
			std::getline(std::cin, in);
			if (in == "q" || in == "quit")
			{
				dialog = false;
				server_socket.shutdown(server_socket.shutdown_both);
				server_socket.close();
				context.stop();
			}
			else
			{
				buffer_string(send_buf, in);
				boost::asio::async_write(server_socket,
					send_buf,
					boost::asio::bind_executor(strand, std::bind(&send_handler,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
		}

#elif defined(BUILD_CLIENT)

		boost::asio::ip::tcp::socket client_socket(context);
		boost::asio::ip::tcp::resolver resolver(context);
		boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(IP.to_string(), std::to_string(PORT));
		
		boost::asio::async_connect(client_socket, endpoints, boost::asio::bind_executor(strand, std::bind(&client_handle_connect, std::ref(client_socket), boost::asio::placeholders::error)));

		while (dialog)
		{
			std::getline(std::cin, in);
			if (in == "q" || in == "quit")
			{
				dialog = false;
				client_socket.shutdown(client_socket.shutdown_both);
				client_socket.close();
				context.stop();
			}
			else
			{
				buffer_string(send_buf, in);
				boost::asio::async_write(client_socket, 
					send_buf, 
					boost::asio::bind_executor(strand, std::bind(&send_handler,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
			}
		}

#endif
		network_thread.join();
	}
	catch (const std::exception& e)
	{
		std::cerr << "exception thrown: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}