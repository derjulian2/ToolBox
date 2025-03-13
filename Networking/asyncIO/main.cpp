#include "asyncIO/Server.hpp"
#include "asyncIO/Client.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	try
	{
		std::filesystem::path file_path;
		std::string ip_address = "127.0.0.1";
		std::string port_num = "0";
		if (argc == 5)
		{
			if (std::string(argv[1]) == "-c")
			{
				ip_address = std::string(argv[2]);
				port_num = std::string(argv[3]);
				file_path = std::string(argv[4]);
			}
			else
			{
				std::cout << "invalid arguments" << std::endl;
				return -1;
			}
		}
		else if (argc == 3)
		{
			if (std::string(argv[1]) == "-s")
			{
				file_path = std::string(argv[2]);
			}
			else
			{
				std::cout << "invalid arguments" << std::endl;
				return -1;
			}
		}
		else
		{
			std::cout << "invalid arguments" << std::endl;
			return -1;
		}

		if (std::string(argv[1]) == "-c")
		{
			tcp::Client client(ip_address, port_num);

			std::cout << "client trying to connect to server" << std::endl;
			std::cout << "timeout in 5 seconds..." << std::endl;
			if (!client.connect(0, std::chrono::seconds(5)))
			{
				std::cout << "connection timed out" << std::endl;
				return -1;
			}
			else
			{
				std::cout << client.getLastLog() << std::endl;
			}
			// test connection via ping
			if (!client.ping())
			{
				std::cout << "connection to server faulty, ping failed" << std::endl;
				return -1;
			}
			// send file
			if (!client.sendfile(file_path))
			{
				std::cout << "failed to send file: " << file_path << std::endl << client.getLastLog() << std::endl;
				return -1;
			}
			else
			{
				std::cout << "successfully transferred file: " << file_path << " to server " << std::endl;
			}
			std::cout << client.getLog() << std::endl;
		}
		else if (std::string(argv[1]) == "-s")
		{
			tcp::Server server(port_num);
			
			std::cout << server.getLastLog() << std::endl;
			std::cout << "server listening for incoming connections" << std::endl;
			std::cout << "timeout in 120 seconds..." << std::endl;
			
			if (!server.accept(0, std::chrono::seconds(120)))
			{
				std::cout << "connection timed out" << std::endl;
				return -1;
			}
			else
			{
				std::cout << server.getLastLog() << std::endl;
			}

			// test connection via ping
			if (!server.receiveping())
			{
				std::cout << "connection to client faulty, ping failed" << std::endl;
				return -1;
			}
			// receive file
			if (!server.receivefile(file_path))
			{
				std::cout << "failed to receive file from client" << std::endl << server.getLastLog() << std::endl;
				return -1;
			}
			else
			{
				std::cout << "successfully wrote file to " << file_path << std::endl;
			}
			std::cout << server.getLog() << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "exception thrown: " << e.what() << std::endl;
		return -1;
	}
	return 0;
}