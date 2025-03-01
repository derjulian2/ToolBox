//////////////////////////////////////////////////
#ifndef WINTRANSFER_H
#define WINTRANSFER_H
//////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <filesystem>
//////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
//////////////////////////////////////////////////
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
//////////////////////////////////////////////////
#define UTIL_MOD_TIMESTAMP
#define UTIL_MOD_FILEMANIP
#include "utilitylib.hpp"
//////////////////////////////////////////////////
/*
* allows windows-specific socket-communication between applications.
*
* make sure to call NetworkStartup() beforehand and NetworkCleanup() afterwards.
*/
//////////////////////////////////////////////////
#define RECEIVE_BUFFER_MSG_LEN  0b111111111
#define RECEIVE_BUFFER_FILE_LEN 0b1111111111111111111111111
//////////////////////////////////////////////////
namespace wtr
{
	//////////////////////////////////////////////////

	bool NetworkStartup();
	void NetworkCleanup();

	bool getLocalIpv4Address(std::string&);

	//////////////////////////////////////////////////
	/*
	* simple server that manages incoming client connections
	* is designed to only allow one client connection at a time
	*/
	class SocketServer
	{
	public:
		SocketServer() = default;
		~SocketServer();
		/*
		* starts the server on the specififed ip_address and port. pass 0 to the port to
		* dynamically get a free port from the OS.
		*/
		bool Startup(const std::string& ip_address, uint16_t port = 0);
		void Shutdown();
		/*
		* [blocking]
		* should be done in another thread?
		* accepts an incoming client connection
		*/
		bool AcceptClient();

		bool Send(const std::string&);
		int Receive(std::string& out, bool largebuf = false);

		bool DataReceived() const;
		bool ClientCanRead() const;

		std::string getLastLog() const;
		std::string getLog() const;
		std::string getIp() const;
		uint16_t getPort() const;
		std::string getClientInfo() const;
		bool isOnline() const;
		bool isConnected() const;

	private:
		void Log(const std::string&);
		u_long blocking = 1;

		SOCKET server_socket = INVALID_SOCKET;
		SOCKET client_socket = INVALID_SOCKET;

		std::string log;
		std::string last_log;

		std::string ip_address;
		uint16_t port = NULL;
		bool online = false;

	};

	//////////////////////////////////////////////////
	
	class SocketClient
	{
	public:
		SocketClient() = default;
		~SocketClient();

		bool Connect(const std::string& ip_address, uint32_t port);
		void Shutdown();
		
		bool Send(const std::string&);
		int Receive(std::string& out, bool largebuf = false);

		bool DataReceived() const;
		bool ServerCanRead() const;

		std::string getLog() const;
		std::string getLastLog() const;
		std::string getIp() const;
		uint16_t getPort() const;
		bool isConnected() const;

	private:
		void Log(const std::string&);
		u_long blocking = 1;

		SOCKET client_socket = INVALID_SOCKET;

		std::string log;
		std::string last_log;

		std::string ip_address;
		uint16_t port = NULL;
		bool connected = false;

	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////