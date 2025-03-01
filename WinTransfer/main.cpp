#include "WinTransfer.hpp"
#include "CmdDialog.hpp"

#include <thread>
#include <mutex>

std::string client_rec_buf;
std::string server_rec_buf;

static inline int client_recv(wtr::SocketClient* socket)
{
    bool con = true;
    while (con)
    {
        if (socket->DataReceived())
        {
            int res = socket->Receive(client_rec_buf);
            if (res == 0 || res == -1)
            {
                con = false;
            }
        }
    }
    return 0;
}

static inline int client()
{
    std::string ip_address = "192.168.2.126";
    std::string port_in;
    uint16_t port;

    std::getline(std::cin, port_in);
    port = std::stoi(port_in);
    wtr::SocketClient client;

    if (!client.Connect(ip_address, port))
    {
        std::cerr << "unable to connect to server: " << client.getLastLog() << std::endl;
        return -1;
    }
    std::thread recv_thread(client_recv, &client);

    cmd::Dialog dialog("WinTransfer", cmd::DEFAULT_HELP);

    dialog.AddFunction("q", [&](DIALOG_ARGS)
        {
            if (argcount != 0)
                return;
            client.Shutdown();
            dialog.close();
        });
    dialog.AddFunction("send", [&](DIALOG_ARGS)
        {
            if (argcount != 1)
                return;
            if (client.ServerCanRead())
            {
                client.Send(arguments[0]);
            }
            std::cout << client.getLastLog() << std::endl;
        });
    dialog.AddFunction([]() {
        if (client_rec_buf.size())
        {
            std::cout << client_rec_buf << std::endl;
            client_rec_buf.clear();
        }
        });
    // main thread will handle the dialog
    dialog.query(std::cout, std::cin);
    recv_thread.join();

    return 0;
}

static inline int server_recv(wtr::SocketServer* socket)
{
    bool con = true;
    while (con)
    {
        if (socket->isConnected())
        {
            if (socket->DataReceived())
            {
                int res = socket->Receive(server_rec_buf);
                if (res == 0 || res == -1)
                {
                    con = false;
                }
            }
        }
    }
    return 0;
}

static inline int server_accept(wtr::SocketServer* socket)
{
    while (!socket->isConnected())
    {
        if (socket->AcceptClient())
        {
            std::cout << "client connected!" << std::endl;
        }
    }
    return 0;
}

static inline int server()
{
    std::string ip_address;
    if (!wtr::getLocalIpv4Address(ip_address))
    {
        std::cerr << "WinTransfer failed to determine local ip-address" << std::endl;
        return -1;
    }

    wtr::SocketServer server;
    if (!server.Startup(ip_address))
    {
        std::cerr << server.getLog() << std::endl;
    }
    std::cout << server.getLastLog() << std::endl;
    

    std::thread recv_thread(server_recv, &server);
    std::thread accept_thread(server_accept, &server);

    cmd::Dialog dialog("WinTransfer", cmd::DEFAULT_HELP);

    dialog.AddFunction("q", [&](DIALOG_ARGS)
        {
            if (argcount != 0)
                return;
            server.Shutdown();
            dialog.close();
        });
    dialog.AddFunction("send", [&](DIALOG_ARGS)
        {
            if (argcount != 1)
                return;
            if (server.ClientCanRead())
            {
                server.Send(arguments[0]);
            }
            std::cout << server.getLastLog() << std::endl;
        });
    dialog.AddFunction("client", [&](DIALOG_ARGS)
        {
            if (argcount != 0)
                return;
            std::cout << server.getClientInfo() << std::endl;
        });
    dialog.AddFunction([]() {
        if (server_rec_buf.size())
        {
            std::cout << server_rec_buf << std::endl;
            server_rec_buf.clear();
        }
        });
    // main thread will handle the dialog
    dialog.query(std::cout, std::cin);
    if (accept_thread.joinable())
    {
        accept_thread.join();
    }
    recv_thread.join();

    return 0;
}

int main(int argc, char** argv) 
{
    if (!wtr::NetworkStartup())
    {
        std::cerr << "WinTransfer failed to start" << std::endl;
        return -1;
    }

    std::string cs;
    std::getline(std::cin, cs);

    char c = cs[0];
    if (c == 'c')
    {
        client();
    }
    else if (c == 's')
    {
        server();
    }

    wtr::NetworkCleanup();
    return 0;
}