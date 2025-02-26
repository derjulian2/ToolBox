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

#define RECEIVE_BUFFER_LEN 512

namespace wtr
{
	bool NetworkStartup();
	void NetworkCleanup();

	class ServerSocket
	{
	public:
		ServerSocket();

		bool Listen(const std::string& ip_address, uint64_t port);
		bool Accept();
		bool Close();

		bool SendString(const std::string& str);
		bool SendFile(const std::filesystem::path& path);
		bool Receive(std::string& dest);

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

	private:
		std::string log;
		Status status = BAD;

		std::string ip_address;
		uint64_t port = NULL;

		SOCKET server_socket = INVALID_SOCKET;
		SOCKET accept_socket = INVALID_SOCKET;
	};
	
	class ClientSocket
	{
	public:
		ClientSocket();

		bool Connect(const std::string& ip_address, uint64_t port);
		bool Close();
		
		bool SendString(const std::string& str);
		bool SendFile(const std::filesystem::path& path);
		bool Receive(std::string& dest);

		enum Status
		{
			BAD,
			OPEN,
			CONNECTED,
		};

		std::string getLog() const;
		Status getStatus() const;

	private:
		std::string log;
		Status status = BAD;

		std::string ip_address;
		uint64_t port = NULL;

		SOCKET client_socket = INVALID_SOCKET;
	};

}

#endif