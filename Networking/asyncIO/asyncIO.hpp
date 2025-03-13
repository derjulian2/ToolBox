//////////////////////////////////////////////////
#ifndef PUSH_FILE_HPP
#define PUSH_FILE_HPP
//////////////////////////////////////////////////
#define UTIL_MOD_FILEMANIP
#define UTIL_MOD_TIMESTAMP
#include "utilitylib.hpp"
#include "CmdDialog.hpp"
//////////////////////////////////////////////////
#include <boost/asio.hpp>
//////////////////////////////////////////////////
#include <string>
#include <filesystem>
#include <memory>
#include <list>
#include <functional>
#include <iostream>
#include <thread>
//////////////////////////////////////////////////
/*
* server/client implementation for transferring files over tcp-sockets using boost.asio
* implementation uses boost::asio asynchronous model
* still requires some modular way to pass in send()
* and receive() handlers to customize behaviour of client/server
*/
//////////////////////////////////////////////////
namespace push
{
	//////////////////////////////////////////////////

	using namespace boost::asio::ip;

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

	class Status
	{
	public:
		enum StatusCode
		{
			BAD,
			CONNECTED,
			DISCONNECTED,
			FATAL_ERROR
		};

		StatusCode getStatus() const;
	protected:
		void setStatus(StatusCode);

		StatusCode statuscode = BAD;
	};

	//////////////////////////////////////////////////

	class Connection : public Logging
	{
	public:
		Connection(boost::asio::io_context& ioc, size_t buf_size = 1024u);
		~Connection();
		void close();

		void send(const std::string& str);
		void recv();
	private:
		friend class Server;
		friend class Client;

		void handle_send(const boost::system::error_code&, size_t);
		void handle_recv(const boost::system::error_code&, size_t);

		tcp::socket socket;
		boost::asio::streambuf out_buf;
		boost::asio::streambuf in_buf;
	};

	//////////////////////////////////////////////////
	/*
	* asynchronous boost::asio tcp-server
	*/
	class Server : public Logging, public Status
	{
	public:
		Server(boost::asio::io_context& ioc, const std::string& port = "0");
		Server(const Server&) = delete;
		Server(const Server&&) = delete;
		Server& operator=(const Server&) = delete;
		Server& operator=(const Server&&) = delete;
		void shutdown();

		void listen();
		Connection& getConnection();
	private:
		void handle_accept(const boost::system::error_code&);

		tcp::acceptor acceptor;
		Connection connection;
	};

	//////////////////////////////////////////////////
	/*
	* asynchronous boost::asio tcp-client
	*/
	class Client : public Logging, public Status
	{
	public:
		Client(boost::asio::io_context& ioc, const std::string& ipv4, const std::string& port);
		Client(const Client&) = delete;
		Client(const Client&&) = delete;
		Client& operator=(const Client&) = delete;
		Client& operator=(const Client&&) = delete;
		void shutdown();

		Connection& getConnection();
	private:
		void handle_resolve(const boost::system::error_code&, const boost::asio::ip::tcp::resolver::results_type&);
		void handle_connect(const boost::system::error_code&, const boost::asio::ip::tcp::endpoint&);

		tcp::resolver resolver;
		Connection connection;
	};

	//////////////////////////////////////////////////

	/*
	* uses multiple threads to control a server/client while
	* still letting the networking do it's thing in parallel
	*/

	class ControlPanel
	{
	public:
		ControlPanel() = default;
		ControlPanel(const ControlPanel&) = delete;
		ControlPanel(const ControlPanel&&) = delete;
		ControlPanel& operator=(const ControlPanel&) = delete;
		ControlPanel& operator=(const ControlPanel&&) = delete;

		virtual void start() = 0;
	protected:
		void network_startup();
		boost::asio::io_context ioc;
		std::thread network_thread;
	};

	class ServerControlPanel : public ControlPanel
	{
	public:
		ServerControlPanel(const std::string& port = "0");

		void start() override;
	private:
		std::shared_ptr<push::Server> server = nullptr;
	};

	class ClientControlPanel : public ControlPanel
	{
	public:
		ClientControlPanel(const std::string& ipv4, const std::string& port);

		void start() override;
	private:
		std::shared_ptr<push::Client> client = nullptr;
	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif 
//////////////////////////////////////////////////