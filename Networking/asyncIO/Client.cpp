//////////////////////////////////////////////////
#include "Client.hpp"
//////////////////////////////////////////////////

tcp::Client::Client(const std::string& ip, const std::string& port)
	: ip(ip), port(port)
{
	io_context = std::make_shared<boost::asio::io_context>();
	resolver = std::make_shared<boost::asio::ip::tcp::resolver>(*io_context);
	connection = std::make_shared<Connection>(io_context);
	VALID = true;
}

tcp::Client::~Client()
{
	close();
}

void tcp::Client::close()
{
	if (VALID)
	{
		connection->close();
		VALID = false;
	}
}

void tcp::Client::run()
{
	if (VALID)
	{
		io_context->run();
	}
}

bool tcp::Client::valid() const
{
	return VALID;
}

tcp::Connection& tcp::Client::getConnection()
{
	if (!VALID)
	{
		throw std::runtime_error("tried to access invalid connection");
	}
	return *connection;
}

bool tcp::Client::ping()
{
	if (!VALID)
	{
		return false;
	}
	if (!connection->send("$ping$", 0, std::chrono::milliseconds(0)))
	{
		Log("ping timed out or an error occured");
		return false;
	}
	std::string buf;
	if (!connection->receive(buf, 0, 5, std::chrono::seconds(5)))
	{
		Log("ping timed out or an error occured");
		return false;
	}
	printf("\nbuf: %s\n", buf.c_str());
	if (buf == "$ping$")
	{
		Log("ping successfully sent and received. Connection OK");
		return true;
	}
	Log("ping timed out or an error occured");
	return false;
}

bool tcp::Client::sendfile(const std::filesystem::path& path)
{
	if (!VALID)
	{
		return false;
	}
	std::string recvbuf;
	utility::bytebuffer filebuf;
	if (!utility::readfile(path, filebuf))
	{
		Log("failed to read file");
		return false;
	}
	size_t bytes_sent = 0;
	size_t file_size = filebuf.size();
	Log("file send request initiated");
	Log("file size: " + std::to_string(file_size) + " bytes");
	if (!connection->send("$expect_file$" + std::to_string(file_size), 0, std::chrono::milliseconds(10)))
	{
		Log("error with sending file metadata");
		return false;
	}
	if (!connection->receive(recvbuf, 0, 1, std::chrono::milliseconds(10)))
	{
		Log("file transfer timed out or an error occured");
		return false;
	}
	if (recvbuf != "$expect_file$OK")
	{
		Log("server failed to accept file transfer");
		return false;
	}
	while (bytes_sent < file_size)
	{
		size_t current_bytes_sent = 0;
		if (!connection->send(utility::to_string(filebuf).substr(bytes_sent, PACKET_SIZE), [&](size_t b)
			{
				current_bytes_sent = b;
			}, std::chrono::milliseconds(0)))
		{
			Log("error with transferring file");
			return false;
		}
		if (!connection->receive(recvbuf, 0, 1, std::chrono::milliseconds(10)))
		{
			Log("error with transferring file");
			return false;
		}
		if (recvbuf.substr(0, 15) != "$received_file$" || recvbuf.size() <= 15)
		{
			Log("error with transferring file");
			return false;
		}
		bytes_sent += std::stol(utility::split_string(recvbuf, "$")[1]);
		Log("transferred " + std::to_string(current_bytes_sent) + " bytes, total: " + std::to_string(bytes_sent) + " bytes");
	}
	if (!connection->send("$end_of_file$", 0, std::chrono::milliseconds(0)))
	{
		Log("server failed to accept end-of-file notice");
		return false;
	}
	if (!connection->receive(recvbuf, 0, 1, std::chrono::milliseconds(10)))
	{
		Log("error with transferring file");
		return false;
	}
	if (recvbuf != "$file_OK$")
	{
		Log("error with transferring file");
		return false;
	}
	Log("successfully transferred file");
	return true;
}