//////////////////////////////////////////////////
#include "Connection.hpp"
//////////////////////////////////////////////////

void tcp::Logging::Log(const std::string& str)
{
	this->log.emplace_back(utility::Timestamp::get() + " : " + str);
}

std::string tcp::Logging::getLastLog() const
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

std::string tcp::Logging::getLog() const
{
	std::string res;
	for (const std::string& l : this->log)
	{
		res.append(l + "\n");
	}
	return res;
}

//////////////////////////////////////////////////

tcp::Connection::Connection(std::shared_ptr<boost::asio::io_context> ioc)
{
	io_context = ioc;
	socket = std::make_shared<boost::asio::ip::tcp::socket>(*ioc);
	VALID = true;
}

tcp::Connection::~Connection()
{
	close();
}

void tcp::Connection::close()
{
	if (VALID)
	{
		this->socket->close();
		VALID = false;
	}
}

bool tcp::Connection::valid() const
{
	return VALID;
}

//////////////////////////////////////////////////