#ifndef WINTRANSFER_H
#define WINTRANSFER_H

#include <iostream>
#include <string>
#include <filesystem>

#include <Windows.h>


/*
* allows windows-specific socket-communication between applications.
*
* when using make sure to call NetworkStartup() beforehand and NetworkCleanup() afterwards.
*/

#define RECEIVE_BUFFER_MSG_LEN  0b111111111
#define RECEIVE_BUFFER_FILE_LEN 0b1111111111111111111111111

namespace wtr
{
	bool NetworkStartup();
	void NetworkCleanup();

	std::string getLocalIpv4Address();

	class ServerSocket
	{
	public:
		ServerSocket() = default;
		~ServerSocket();

		bool Open();
		bool Close();

		bool Listen(const std::string& ip_address, uint64_t port = 0);
		bool Accept();

		bool Send(const std::string& str);
		bool Receive(std::string& dest, bool large_buf = false);

		enum Status
		{
			BAD,
			OPEN,
			LISTENING,
			CONNECTION_INCOMING,
			CONNECTED
		};

		std::string getLog() const;
		Status getStatus() const;
		std::string getIp() const;
		uint32_t getPort() const;

	private:
		std::string log;
		Status status = BAD;

		std::string ip_address;
		uint32_t port = NULL;

		SOCKET server_socket = INVALID_SOCKET;
		SOCKET accept_socket = INVALID_SOCKET;
	};
	
	class ClientSocket
	{
	public:
		ClientSocket() = default;
		~ClientSocket();

		bool Open();
		bool Close();

		bool Connect(const std::string& ip_address, uint64_t port);
		
		bool Send(const std::string& str);
		bool Receive(std::string& dest, bool large_buf = false);

		enum Status
		{
			BAD,
			OPEN,
			CONNECTED,
		};

		std::string getLog() const;
		Status getStatus() const;
		std::string getIp() const;
		uint32_t getPort() const;

	private:
		std::string log;
		Status status = BAD;

		std::string ip_address;
		uint32_t port = NULL;

		SOCKET client_socket = INVALID_SOCKET;
	};

}

#endif