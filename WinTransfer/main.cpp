#include "WinTransfer.hpp"
#include "CmdDialog.hpp"
#define UTIL_MOD_FILEMANIP
#include "utilitylib.hpp"

static inline int wintransfer_main(int argc, char** argv)
{
    if (argc != 2)
    {
        if (argc == 1)
        {
            std::cerr << "WinTransfer error at " << argv[0] << ": invalid arguments" << std::endl;
        }
        else
        {
            std::cerr << "WinTransfer error: invalid arguments" << std::endl;
        }
        return -1;
    }

    if (!wtr::NetworkStartup())
    {
        return -1;
    }

    if (std::string(argv[1]) == "-c") // wintransfer in client mode
    {
        wtr::ClientSocket client;
        if (!client.Open())
        {
            std::cerr << "WinTransfer error at " << argv[0] << ": failed to open client socket";
            std::cerr << "socket logs: " << std::endl << client.getLog() << std::endl;
            return -1;
        }

        CmdDialog dialog("WinTransfer", CmdDialogFlags::DEFAULT_HELP | CmdDialogFlags::DEFAULT_QUIT);

        dialog.AddCmdDialogFunction("connect", "connect to WinTransfer server", { "[ipv4] ip-address", "[int] port"},
            [&](const ArgCount& c, const Arguments& args)
            {
                if (c != 2)
                {
                    std::cerr << "connect: invalid arguments" << std::endl;
                    return;
                }
                client.Connect(args[0].c_str(), std::stoi(args[1]));
            });
        dialog.AddCmdDialogFunction("send_file", "send files to connected server", { "[path] path to file" },
            [&](const ArgCount& c, const Arguments& args)
            {
                if (c != 2)
                {
                    std::cerr << "connect: invalid arguments" << std::endl;
                    return;
                }
            });
        dialog.AddCmdDialogFunction("send_msg", "send messages to connected server", { "[string] message" },
            [&](const ArgCount& c, const Arguments& args)
            {
                if (c != 1)
                {
                    std::cerr << "send_msg: invalid arguments" << std::endl;
                    return;
                }
                client.Send(args[0]);
            });
        dialog.AddCmdDialogFunction("receive", "collect the server's sent data", { "[none]" },
            [&](const ArgCount& c, const Arguments& args)
            {
                if (c != 0)
                {
                    std::cerr << "receive: invalid arguments" << std::endl;
                    return;
                }
                std::string receive;
                client.Receive(receive);
                std::cout << receive << std::endl;
            });

        dialog.QueryInput();
    }
    else if (std::string(argv[1]) == "-s") // wintransfer in server mode
    {
        wtr::ServerSocket server;
        if (!server.Open())
        {
            std::cerr << "WinTransfer error at " << argv[0] << ": failed to open server socket";
            std::cerr << "socket logs: " << std::endl << server.getLog() << std::endl;
            return -1;
        }
        std::string ip_address;
        try
        {
            ip_address = wtr::getLocalIpv4Address();
        }
        catch (const std::exception& except)
        {
            std::cerr << "WinTransfer error at " << argv[0] << ": " << except.what() << std::endl;
            return -1;
        }
        if (!server.Listen(ip_address))
        {
            std::cerr << "WinTransfer error at " << argv[0] << ": failed in ServerSocket::Listen()" << std::endl;
            std::cerr << "socket logs: " << std::endl << server.getLog() << std::endl;
            return -1;
        }
        std::cout << "started WinTransfer server at " << server.getIp() << " on port " << server.getPort() << std::endl;
        std::cout << "waiting on incoming client connection..." << std::endl;
        if (!server.Accept())
        {
            std::cerr << "WinTransfer error at " << argv[0] << ": failed in ServerSocket::Accept()" << std::endl;
            std::cerr << "socket logs: " << std::endl << server.getLog() << std::endl;
            return -1;
        }

        CmdDialog dialog("WinTransfer", CmdDialogFlags::DEFAULT_HELP | CmdDialogFlags::DEFAULT_QUIT);

        dialog.AddCmdDialogFunction("send_file", "send files to connected client", { "[path] path to file" },
            [&](const ArgCount& c, const Arguments& args)
            {

            });
        dialog.AddCmdDialogFunction("send_msg", "send messages to connected client", { "[string] message" },
            [&](const ArgCount& c, const Arguments& args)
            {
                if (c != 1)
                {
                    std::cerr << "send_msg: invalid arguments" << std::endl;
                    return;
                }
                server.Send(args[0]);
            });
        dialog.AddCmdDialogFunction("receive", "collect the client's sent data", { "[none]" },
            [&](const ArgCount& c, const Arguments& args)
            {
                if (c != 0)
                {
                    std::cerr << "receive: invalid arguments" << std::endl;
                    return;
                }
                std::string receive;
                server.Receive(receive);
                std::cout << receive << std::endl;
            });

        dialog.QueryInput();
    }

    wtr::NetworkCleanup();
    return 0;
}

int main(int argc, char** argv) 
{
    return wintransfer_main(argc, argv);
    return 0;
}