//////////////////////////////////////////////////
#include "WinTransfer.hpp"
//////////////////////////////////////////////////
bool wtr::NetworkStartup()
{
    WSAData wsa_data = {};
    int wsa_error = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (wsa_error)
    {
        std::cerr << "WinSock failed with error code: " << wsa_error << std::endl;
        return false;
    }
    return true;
}
void wtr::NetworkCleanup()
{
    WSACleanup();
}
bool wtr::getLocalIpv4Address(std::string& outbuf)
{
    // mixing C and C++ code, maybe not good
    PIP_ADAPTER_INFO pAdapterInfo = static_cast<IP_ADAPTER_INFO*>(malloc(sizeof(IP_ADAPTER_INFO)));
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    bool res = false;

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
    {
        if (pAdapterInfo)
        {
            outbuf = std::string(pAdapterInfo->IpAddressList.IpAddress.String);
            res = true;
        }
        else
        {
            res = false;
        }
    }
    else
    {
        res = false;
    }
    
    free(pAdapterInfo);
    pAdapterInfo = nullptr;

    return res;
}
//////////////////////////////////////////////////
wtr::SocketServer::~SocketServer()
{
    Shutdown();
}
bool wtr::SocketServer::Startup(const std::string& ip_address, uint16_t port)
{
    addrinfo hints{};
    addrinfo* result = nullptr;
    int error = 0;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
        
    error = getaddrinfo(ip_address.c_str(), std::to_string(port).c_str(), &hints, &result);
    if (error != NO_ERROR)
    {
        Log("server startup failed at getaddrinfo() with error: " + std::to_string(error));
        return false;
    }
   
    server_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
    if (server_socket == INVALID_SOCKET)
    {
        Log("server startup failed at socket() with error: " + std::to_string(WSAGetLastError()));
        if (result)
        {
            freeaddrinfo(result);
            result = nullptr;
        }
        Shutdown();
        return false;
    }
    if (ioctlsocket(server_socket, FIONBIO, &blocking) == SOCKET_ERROR)
    {
        Log("server startup failed at ioctlsocket() with error: " + std::to_string(WSAGetLastError()));
        if (result)
        {
            freeaddrinfo(result);
            result = nullptr;
        }
        Shutdown();
        return false;
    }

    error = bind(server_socket, result->ai_addr, result->ai_addrlen);
    if (error == SOCKET_ERROR)
    {
        Log("server startup failed at bind() with error: " + std::to_string(WSAGetLastError()));
        if (result)
        {
            freeaddrinfo(result);
            result = nullptr;
        }
        Shutdown();
        return false;
    }

    error = listen(server_socket, SOMAXCONN);
    if (error == SOCKET_ERROR)
    {
        Log("server startup failed at listen() with error: " + std::to_string(WSAGetLastError()));
        if (result)
        {
            freeaddrinfo(result);
            result = nullptr;
        }
        Shutdown();
        return false;
    }

    if (port == 0)
    {
        sockaddr_in addr_info{};
        int namelen = sizeof(sockaddr_in);
        if (getsockname(server_socket, reinterpret_cast<sockaddr*>(&addr_info), &namelen) == SOCKET_ERROR)
        {
            Log("server startup failed at getsockname() with error: " + std::to_string(WSAGetLastError()));
            if (result)
            {
                freeaddrinfo(result);
                result = nullptr;
            }
            Shutdown();
            return false;
        }
        this->port = ntohs(addr_info.sin_port);
    }
    else
    {
        this->port = port;
    }
    this->ip_address = ip_address;

    if (result)
    {
        freeaddrinfo(result);
        result = nullptr;
    }
    Log("server startup successful. listening for incoming clients at " + this->ip_address + " on port " + std::to_string(this->port));
    online = true;
    return true;
}
void wtr::SocketServer::Shutdown()
{
    if (online)
    {
        if (client_socket != INVALID_SOCKET)
        {
            shutdown(client_socket, SD_BOTH);
            closesocket(client_socket);
            client_socket = INVALID_SOCKET;
        }
        if (server_socket != INVALID_SOCKET)
        {
            shutdown(server_socket, SD_BOTH);
            closesocket(server_socket);
            server_socket = INVALID_SOCKET;
        }
    }
    online = false;
}
bool wtr::SocketServer::AcceptClient()
{
    if (!online)
    {
        return false;
    }

    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == INVALID_SOCKET)
    {
        Log("server could not accept incoming client");
        return false;
    }
    if (ioctlsocket(client_socket, FIONBIO, &blocking) == SOCKET_ERROR)
    {
        Log("server could not accept incoming client");
        return false;
    }

    Log("server accepted new client connection");
    return true;
}
bool wtr::SocketServer::Send(const std::string& str)
{
    if (!online || client_socket == INVALID_SOCKET)
    {
        return false;
    }

    int error = send(client_socket, str.c_str(), str.size(), 0);
    if (error == SOCKET_ERROR)
    {
        Log("failed to send " + std::to_string(str.size()) + " bytes");
        return false;
    }
    else
    {
        Log("sent " + std::to_string(str.size()) + " bytes");
        return true;
    }
}
int wtr::SocketServer::Receive(std::string& out, bool largebuf)
{
    if (!online)
    {
        return -1; // error
    }
    char* receive_buf = nullptr;
    long length = 0;
    if (largebuf)
    {
        receive_buf = new char[RECEIVE_BUFFER_FILE_LEN];
        length = RECEIVE_BUFFER_FILE_LEN;
        std::fill(&receive_buf[0], &receive_buf[RECEIVE_BUFFER_FILE_LEN - 1], 0);
    }
    else
    {
        receive_buf = new char[RECEIVE_BUFFER_MSG_LEN];
        length = RECEIVE_BUFFER_MSG_LEN;
        std::fill(&receive_buf[0], &receive_buf[RECEIVE_BUFFER_MSG_LEN - 1], 0);
    }

    int error = recv(client_socket, receive_buf, length, 0);
    if (error == SOCKET_ERROR)
    {
        Log("failed to receive message from server");
        if (receive_buf)
        {
            delete[] receive_buf;
        }
        return -1; // error
    }
    else if (error > 0)
    {
        if (receive_buf)
        {
            std::string temp;
            temp.append(receive_buf);
            temp.shrink_to_fit();
            out.append(temp);
            std::fill(&receive_buf[0], &receive_buf[length - 1], 0);
            Log("received " + std::to_string(temp.size()) + " bytes");
        }
        if (receive_buf)
        {
            delete[] receive_buf;
        }
        return 1; // data received
    }
    else if (error == 0)
    {
        Log("connection has been closed");
        if (receive_buf)
        {
            delete[] receive_buf;
        }
        return 0; // connection has been closed
    }
}
bool wtr::SocketServer::DataReceived() const
{
    if (client_socket == INVALID_SOCKET)
    {
        return false;
    }

    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(client_socket, &rfd);

    int ret = select(client_socket, &rfd, 0, 0, 0);

    if (rfd.fd_count)
    {
        if (FD_ISSET(client_socket, &rfd))
        {
            return true;
        }
    }
    return false;
}
bool wtr::SocketServer::ClientCanRead() const
{
    if (client_socket == INVALID_SOCKET)
    {
        return false;
    }

    fd_set wfd;
    FD_ZERO(&wfd);
    FD_SET(client_socket, &wfd);

    int ret = select(client_socket, 0, &wfd, 0, 0);

    if (wfd.fd_count)
    {
        if (FD_ISSET(client_socket, &wfd))
        {
            return true;
        }
    }
    return false;
}
std::string wtr::SocketServer::getLastLog() const
{
    return last_log;
}
std::string wtr::SocketServer::getLog() const
{
    return log;
}
std::string wtr::SocketServer::getIp() const
{
    return ip_address;
}
uint16_t wtr::SocketServer::getPort() const
{
    return port;
}
std::string wtr::SocketServer::getClientInfo() const
{
    if (!online)
    {
        return std::string("no client information available, server is offline");
    }

    std::string res = "client information: \n";
    sockaddr_in addr_info{};
    int namelen = sizeof(sockaddr_in);
    if (getsockname(client_socket, reinterpret_cast<sockaddr*>(&addr_info), &namelen) == SOCKET_ERROR)
    {
        res.append("no information available");
    }
    else
    {
        res.append(" family: " + std::to_string(addr_info.sin_family));
        res.append(", address: " + 
            std::to_string(addr_info.sin_addr.S_un.S_un_b.s_b1) + "." +
            std::to_string(addr_info.sin_addr.S_un.S_un_b.s_b2) + "." +
            std::to_string(addr_info.sin_addr.S_un.S_un_b.s_b3) + "." +
            std::to_string(addr_info.sin_addr.S_un.S_un_b.s_b4));
        res.append(", port: " + std::to_string(addr_info.sin_port));
    }
    res.append("\n");
    return res;
}
bool wtr::SocketServer::isOnline() const
{
    return online;
}
bool wtr::SocketServer::isConnected() const
{
    if (client_socket == INVALID_SOCKET)
    {
        return false;
    }
    return true;
}

void wtr::SocketServer::Log(const std::string& str)
{
    log.append("<" + utility::Timestamp::get() + "> " + str + "\n");
    last_log = str;
}
//////////////////////////////////////////////////
wtr::SocketClient::~SocketClient()
{
    Shutdown();
}
bool wtr::SocketClient::Connect(const std::string& ip_address, uint32_t port)
{
    addrinfo hints{};
    addrinfo* result = nullptr;
    int error = 0;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    error = getaddrinfo(ip_address.c_str(), std::to_string(port).c_str(), &hints, &result);
    if (error != NO_ERROR)
    {
        Log("server connection failed at getaddrinfo() with error: " + std::to_string(error));
        return false;
    }

    client_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (client_socket == INVALID_SOCKET)
    {
        Log("client connection failed at socket() with error code: " + std::to_string(WSAGetLastError()));
        if (result)
        {
            freeaddrinfo(result);
            result = nullptr;
        }
        Shutdown();
        return false;
    }

    if (ioctlsocket(client_socket, FIONBIO, &blocking) == SOCKET_ERROR)
    {
        Log("client connection failed at ioctlsocket() with error code: " + std::to_string(WSAGetLastError()));
        if (result)
        {
            freeaddrinfo(result);
            result = nullptr;
        }
        Shutdown();
        return false;
    }

    error = connect(client_socket, result->ai_addr, result->ai_addrlen);
    if (error == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            Log("client connection failed at connect() with error code: " + std::to_string(WSAGetLastError()));
            if (result)
            {
                freeaddrinfo(result);
                result = nullptr;
            }
            Shutdown();
            return false;
        }
    }

    if (result)
    {
        freeaddrinfo(result);
        result = nullptr;
    }
    connected = true;
    return true;
}
void wtr::SocketClient::Shutdown()
{
    if (connected)
    {
        if (client_socket != INVALID_SOCKET)
        {
            shutdown(client_socket, SD_BOTH);
            closesocket(client_socket);
            client_socket = INVALID_SOCKET;
        }
    }
    connected = false;
}
bool wtr::SocketClient::Send(const std::string& str)
{
    if (!connected)
    {
        return false;
    }

    int error = send(client_socket, str.c_str(), str.size(), 0);
    if (error == SOCKET_ERROR)
    {
        Log("failed to send " + std::to_string(str.size()) + " bytes");
        return false;
    }
    else
    {
        Log("sent " + std::to_string(str.size()) + " bytes");
        return true;
    }
}
int wtr::SocketClient::Receive(std::string& out, bool largebuf)
{
    if (!connected)
    {
        return -1;
    }
    char* receive_buf = nullptr;
    long length = 0;
    if (largebuf)
    {
        receive_buf = new char[RECEIVE_BUFFER_FILE_LEN];
        length = RECEIVE_BUFFER_FILE_LEN;
        std::fill(&receive_buf[0], &receive_buf[RECEIVE_BUFFER_FILE_LEN - 1], 0);
    }
    else
    {
        receive_buf = new char[RECEIVE_BUFFER_MSG_LEN];
        length = RECEIVE_BUFFER_MSG_LEN;
        std::fill(&receive_buf[0], &receive_buf[RECEIVE_BUFFER_MSG_LEN - 1], 0);
    }

    int error = recv(client_socket, receive_buf, length, 0);
    if (error == SOCKET_ERROR)
    {
        Log("failed to receive message from server");
        if (receive_buf)
        {
            delete[] receive_buf;
        }
        return -1;
    }
    else if (error > 0)
    {
        if (receive_buf)
        {
            std::string temp;
            temp.append(receive_buf);
            temp.shrink_to_fit();
            out.append(temp);
            std::fill(&receive_buf[0], &receive_buf[length - 1], 0);
            Log("received " + std::to_string(temp.size()) + " bytes");
            if (receive_buf)
            {
                delete[] receive_buf;
            }
            return 1; // data received
        }
    }
    else if (error == 0)
    {
        Log("connection has been closed");
        if (receive_buf)
        {
            delete[] receive_buf;
        }
        return 0; // connection has been closed
    }
}
bool wtr::SocketClient::DataReceived() const
{
    if (!connected)
    {
        return false;
    }

    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(client_socket, &rfd);


    int ret = select(client_socket, &rfd, 0, 0, 0);

    if (rfd.fd_count)
    {
        if (FD_ISSET(client_socket, &rfd))
        {
            return true;
        }
    }
    return false;
}
bool wtr::SocketClient::ServerCanRead() const
{
    if (!connected)
    {
        return false;
    }

    fd_set wfd;
    FD_ZERO(&wfd);
    FD_SET(client_socket, &wfd);

    int ret = select(client_socket, 0, &wfd, 0, 0);

    if (wfd.fd_count)
    {
        if (FD_ISSET(client_socket, &wfd))
        {
            return true;
        }
    }
    return false;
}
std::string wtr::SocketClient::getLog() const
{
    return log;
}
std::string wtr::SocketClient::getLastLog() const
{
    return last_log;
}
std::string wtr::SocketClient::getIp() const
{
    return ip_address;
}
uint16_t wtr::SocketClient::getPort() const
{
    return port;
}
bool wtr::SocketClient::isConnected() const
{
    return connected;
}
void wtr::SocketClient::Log(const std::string& str)
{
    log.append("<" + utility::Timestamp::get() + "> " + str + "\n");
    last_log = str;
}
//////////////////////////////////////////////////