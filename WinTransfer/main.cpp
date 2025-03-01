#include "WinTransfer.hpp"
#include "CmdDialog.hpp"

static inline int client()
{
    std::string ip_address = "192.168.2.126";
    std::string port_in;
    uint16_t port;

    std::cout << "port number: ";
    std::getline(std::cin, port_in);
    std::cout << std::endl;
    try
    {
        port = std::stoi(port_in);
    }
    catch (std::exception e)
    {
        return -1;
    }
    wtr::SocketClient client;

    if (!client.Connect(ip_address, port))
    {
        std::cerr << "unable to connect to server: " << client.getLastLog() << std::endl;
        return -1;
    }

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
            std::string filebuf;
            if (!utility::readfile(arguments[0], filebuf))
            {
                return;
            }
            if (client.ServerCanRead())
            {
                client.Send(filebuf);
            }
            std::cout << client.getLastLog() << std::endl;
        });

    dialog.query(std::cout, std::cin);

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

    bool session = true;
    bool retry = false;

    while (session)
    {
        wtr::SocketServer server;
        if (!server.Startup(ip_address))
        {
            std::cerr << server.getLog() << std::endl;
            return -1;
        }
        std::cout << server.getLastLog() << std::endl;
        std::cout << "Timeout in 60s" << std::endl;

        utility::execute_for_ms([&](bool& quit) {
            if (!server.isConnected())
            {
                if (server.AcceptClient())
                {
                    std::cout << server.getLastLog() << std::endl;
                }
            }
            else
            {
                quit = true;
            }
            }, std::chrono::milliseconds(1000 * 60));

        if (!server.isConnected())
        {
            cmd::YesNoAll dialog("server established no client connection. retry?",
                [&]() {std::cout << "retrying..." << std::endl; retry = true; }, [&]() { std::cout << "exiting..." << std::endl; session = false; });
        }
        if (retry)
        {
            retry = false;
            continue;
        }
        std::cout << "server waiting for request" << std::endl;
        std::cout << "Timeout in 60s" << std::endl;

        std::string out;
        utility::execute_for_ms([&](bool& quit) {
            if (server.DataReceived())
            {
                server.Receive(out, true);
                quit = true;
            }
            }, std::chrono::milliseconds(1000 * 60));

        if (out.size())
        {
            utility::writetofile("file.txt", out);
            std::cout << "wrote " << out.size() << " bytes to file" << std::endl;
        }
        else
        {
            cmd::YesNoAll dialog("server established no client connection. retry?",
                [&]() {std::cout << "retrying..." << std::endl; retry = true; }, [&]() { std::cout << "exiting..." << std::endl; session = false; });
        }
        if (retry)
        {
            retry = false;
            continue;
        }

        session = false;
    }

    return 0;
}

int main(int argc, char** argv) 
{
    if (!wtr::NetworkStartup())
    {
        std::cerr << "WinTransfer failed to start" << std::endl;
        return -1;
    }

    std::cout << "[c]lient or [s]erver? ";
    std::string cs;
    std::getline(std::cin, cs);
    std::cout << std::endl;

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