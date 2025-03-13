//////////////////////////////////////////////////
#include "Server.hpp"
//////////////////////////////////////////////////

tcp::Server::Server(const std::string& port)
{
	io_context = std::make_shared<boost::asio::io_context>();
	acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(*io_context,
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(port)));
	connection = std::make_shared<Connection>(io_context);
	VALID = true;
	Log("server startet at " +
		acceptor->local_endpoint().address().to_string()
		+ " on port " +
		std::to_string(acceptor->local_endpoint().port()));
}

tcp::Server::~Server()
{
	close();
}

void tcp::Server::close()
{
	if (VALID)
	{
		connection->close();
		VALID = false;
	}
}

void tcp::Server::run()
{
	if (VALID)
	{
		io_context->run();
	}
}

bool tcp::Server::valid() const
{
	return VALID;
}

tcp::Connection& tcp::Server::getConnection()
{
	if (!VALID)
	{
		throw std::runtime_error("tried to access invalid connection");
	}
	return *connection;
}

bool tcp::Server::receiveping()
{
	if (!VALID)
	{
		return false;
	}
	std::string recv_buf;
	if (!connection->receive(recv_buf, 0, 1, std::chrono::seconds(5)))
	{
		Log("error with receiving ping");
		return false;
	}
	if (recv_buf != "$ping$")
	{
		Log("error with receiving ping");
		return false;
	}
	if (!connection->send("$ping$", 0, std::chrono::milliseconds(0)))
	{
		Log("error with receiving ping");
		return false;
	}
	Log("successfully received and sent back ping");
	return true;
}

bool tcp::Server::receivefile(const std::filesystem::path& write_path)
{
	if (!VALID)
	{
		return false;
	}
	std::string recvbuf;
	utility::bytebuffer filebuf;

	size_t bytes_recv = 0;
	size_t file_size = 0;

	if (!connection->receive(recvbuf, 0, 1, std::chrono::milliseconds(10)))
	{
		Log("receive file timed out or error -1");
		return false;
	}
	if (recvbuf.substr(0, 13) != "$expect_file$" || recvbuf.size() <= 13)
	{
		Log("receive file timed out or error -2");
		return false;
	}
	Log("file transfer request initiated");
	file_size = std::stol(utility::split_string(recvbuf, "$")[1]);
	if (!connection->send("$expect_file$OK", 0, std::chrono::milliseconds(0)))
	{
		Log("receive file timed out or error -3");
		return false;
	}
	Log("expected file size: " + std::to_string(file_size) + " bytes");
	while (bytes_recv < file_size)
	{
		size_t current_bytes_recv = 0;
		if (!connection->receive(recvbuf, 
			[&](size_t b)
			{
				current_bytes_recv = b;
				bytes_recv += b;
			}, 1, std::chrono::milliseconds(10)))
		{
			Log("receive file timed out or error -4");
			return false;
		}
		utility::bytebuffer received_data_buf = utility::to_buffer(recvbuf);
		filebuf.insert(filebuf.cend(), received_data_buf.cbegin(), received_data_buf.cend());
		if (!connection->send("$received_file$" + std::to_string(current_bytes_recv), 0, std::chrono::milliseconds(0)))
		{
			Log("receive file timed out or error -5");
			return false;
		}
		Log("transferred " + std::to_string(current_bytes_recv) + " bytes, total: " + std::to_string(bytes_recv) + " bytes");

	}
	if (!connection->receive(recvbuf, 0, 1, std::chrono::milliseconds(10)))
	{
		Log("receive file timed out or error -6");
		return false;
	}
	if (recvbuf != "$end_of_file$")
	{
		Log("receive file timed out or error -7");
		return false;
	}
	if (!connection->send("$file_OK$", 0, std::chrono::milliseconds(0)))
	{
		Log("receive file timed out or error -8");
		return false;
	}
	if (!utility::overwritefile(write_path, filebuf))
	{
		Log("receive file failed to write to file -9");
		return false;
	}
	Log("successfully transferred file");
	return true;
}