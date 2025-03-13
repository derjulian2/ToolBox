//////////////////////////////////////////////////
#include "PushFile.hpp"
//////////////////////////////////////////////////

void push::Logging::Log(const std::string& str)
{
	this->log.emplace_back(utility::Timestamp::get() + " : " + str);
}

std::string push::Logging::getLastLog() const
{
	if (this->log.size())
	{
		return this->log.back();
	}
	else
	{
		return std::string();
	}
}

std::string push::Logging::getLog() const
{
	std::string res;
	for (const std::string& l : this->log)
	{
		res.append(l + "\n");
	}
	return res;
}

//////////////////////////////////////////////////

push::Status::StatusCode push::Status::getStatus() const
{
	return statuscode;
}

void push::Status::setStatus(StatusCode c)
{
	this->statuscode = c;
}

//////////////////////////////////////////////////

push::Connection::Connection(boost::asio::io_context& ioc, size_t buf_size)
	: socket(ioc)
{

}

push::Connection::~Connection()
{
	close();
}

void push::Connection::close()
{
	socket.shutdown(boost::asio::socket_base::shutdown_both);
	socket.close();
}

void push::Connection::send(const std::string& str)
{
	try
	{
		out_buf.consume(out_buf.size());
		out_buf.sputn(str.data(), str.size());
		boost::asio::async_write(this->socket,
			out_buf,
			std::bind(&Connection::handle_send,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
	catch (const std::exception& e)
	{
		Log("error in sending on connection: " + std::string(e.what()));
	}
}

void push::Connection::recv()
{
	try
	{
		boost::asio::async_read(this->socket,
			in_buf,
			boost::asio::transfer_at_least(1u),
			std::bind(&Connection::handle_recv,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
	catch (const std::exception& e)
	{
		Log("error in receiving on connection: " + std::string(e.what()));
	}
}

void push::Connection::handle_send(const boost::system::error_code& ec, size_t bytes)
{
	try
	{
		if (!ec)
		{
			Log(std::to_string(bytes) + " bytes transferred");
		}
		else
		{
			Log("error in handle send on connection: " + ec.message());
		}
	}
	catch (const std::exception& e)
	{
		Log("error in handle send on connection: " + std::string(e.what()));
	}
}

void push::Connection::handle_recv(const boost::system::error_code& ec, size_t bytes)
{
	try
	{
		if (!ec)
		{
			Log(std::to_string(bytes) + " received");
			std::istream is(&out_buf);
			std::string out;
			is >> out;
			std::cout << "message: " << out << std::endl;
		}
		else
		{
			Log("error in handle receive on connection: " + ec.message());
		}
	}
	catch (const std::exception& e)
	{
		Log("error in handle receive on connection: " + std::string(e.what()));
	}
}

//////////////////////////////////////////////////

push::Server::Server(boost::asio::io_context& ioc, const std::string& port)
	: acceptor(ioc, tcp::endpoint(tcp::v4(), std::stoi(port))), connection(ioc)
{
	setStatus(DISCONNECTED);
}

void push::Server::shutdown()
{
	connection.close();
}

void push::Server::listen()
{
	try
	{
		acceptor.async_accept(connection.socket, 
			std::bind(&Server::handle_accept, 
				this, 
				boost::asio::placeholders::error));
	}
	catch (const std::exception& e)
	{
		Log("error in server::listen(): " + std::string(e.what()));
		setStatus(FATAL_ERROR);
	}
}

void push::Server::handle_accept(const boost::system::error_code& ec)
{
	try
	{
		if (!ec)
		{
			Log("new client connection accepted");
			std::cout << getLastLog() << std::endl;
			setStatus(CONNECTED);
		}
		else
		{
			Log("error with accepting new client connection: " + ec.message());
		}
	}
	catch (const std::exception& e)
	{
		Log("error with accepting new client connection:" + std::string(e.what()));
		setStatus(FATAL_ERROR);
	}
}

push::Connection& push::Server::getConnection()
{
	return connection;
}

//////////////////////////////////////////////////

push::Client::Client(boost::asio::io_context& ioc, const std::string& ipv4, const std::string& port)
	: resolver(ioc), connection(ioc)
{
	try
	{
		resolver.async_resolve(ipv4, port,
			std::bind(&Client::handle_resolve,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::results));
		setStatus(DISCONNECTED);
	}
	catch (const std::exception& e)
	{
		Log("error in setting up client connection to server:" + std::string(e.what()));
		setStatus(FATAL_ERROR);
	}
}

void push::Client::shutdown()
{
	connection.close();
}

void push::Client::handle_resolve(const boost::system::error_code& ec, const boost::asio::ip::tcp::resolver::results_type& results)
{
	try
	{
		if (!ec)
		{
			boost::asio::async_connect(connection.socket, results,
				std::bind(&Client::handle_connect,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::endpoint));
		}
		else
		{
			Log("error in establishing connection with server: " + ec.message());
			setStatus(FATAL_ERROR);
		}
	}
	catch (const std::exception& e)
	{
		Log("error in establishing connection with server: " + std::string(e.what()));
		setStatus(FATAL_ERROR);
	}
}

void push::Client::handle_connect(const boost::system::error_code& ec, const boost::asio::ip::tcp::endpoint& ep)
{
	if (!ec)
	{
		Log("successfully established connection with server: at " + ep.address().to_string() + " on port " + std::to_string(ep.port()));
		std::cout << getLastLog() << std::endl;
		setStatus(CONNECTED);
	}
	else
	{
		Log("error in establishing connection with server: " + ec.message());
	}
}

push::Connection& push::Client::getConnection()
{
	return connection;
}

//////////////////////////////////////////////////

void push::ControlPanel::network_startup()
{
	ioc.run();
}

//////////////////////////////////////////////////

push::ServerControlPanel::ServerControlPanel(const std::string& port)
{
	server = std::make_shared<push::Server>(std::ref(ioc), port);
}

void push::ServerControlPanel::start()
{
	// start networking
	server->listen();
	network_thread = std::thread(&ServerControlPanel::network_startup, this);
	// start controlling dialog - somehow connect works but send/recv doesnt -> maybe ioc runs out of work and run() returns??
	while (true)
	{
		std::string in;
		std::getline(std::cin, in);

		server->getConnection().send(in);
		std::cout << server->getConnection().getLog() << std::endl;
		if (ioc.stopped()) // somehow keep ioc alive, but not like this?
		{
			ioc.run(); // this works, but is likely not intended
		}
	}

	server->shutdown();

	if (network_thread.joinable())
	{
		network_thread.join();
	}
}

//////////////////////////////////////////////////

push::ClientControlPanel::ClientControlPanel(const std::string& ipv4, const std::string& port)
{
	client = std::make_shared<push::Client>(std::ref(ioc), ipv4, port);
}

void push::ClientControlPanel::start()
{
	// start networking
	network_thread = std::thread(&ClientControlPanel::network_startup, this);
	// start controlling dialog
	while (true)
	{
		std::string in;
		std::getline(std::cin, in);
		if (in == "i")
			client->getConnection().recv();
		if (ioc.stopped())
		{
			ioc.run();
		}
	}

	client->shutdown();

	if (network_thread.joinable())
	{
		network_thread.join();
	}

}

//////////////////////////////////////////////////