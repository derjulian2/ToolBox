#include "WinTransfer.h"

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

wtr::ServerSocket::ServerSocket()
{
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket == INVALID_SOCKET)
    {
        log.append("socket-initialization failed with error code: " + std::to_string(WSAGetLastError()) + "\n");
    }
    else
    {
        log.append("socket-initialization OK\n");
        status = OPEN;
    }
}

bool wtr::ServerSocket::Listen(const std::string& ip_address, uint64_t port)
{
    if (status != OPEN)
    {
        return false;
    }

    sockaddr_in addr_info = sockaddr_in();
    addr_info.sin_family = AF_INET;
    addr_info.sin_addr.S_un.S_addr = inet_addr(ip_address.c_str());
    addr_info.sin_port = htons(port);

    if (bind(server_socket, reinterpret_cast<sockaddr*>(&addr_info), sizeof(addr_info)) == SOCKET_ERROR)
    {
        log.append("error binding socket: " + std::to_string(WSAGetLastError()) + "\n");
        Close();
        return false;
    }
    else
    {
        this->ip_address = ip_address;
        this->port = port;
        log.append("socket-binding OK\n");
        log.append("server-socket bound to " + ip_address + " at port " + std::to_string(port) + "\n");
    }

    if (listen(server_socket, 1) == SOCKET_ERROR)
    {
        log.append("error listening on socket: " + std::to_string(WSAGetLastError()) + "\n");
        Close();
        return false;
    }
    else
    {
        log.append("listening for new connections...\n");
        status = LISTENING;
        return true;
    }
}

bool wtr::ServerSocket::Accept()
{
    if (status != LISTENING)
    {
        return false;
    }

    accept_socket = accept(server_socket, nullptr, nullptr);

    if (accept_socket == INVALID_SOCKET)
    {
        log.append("error on accepting socket: " + std::to_string(WSAGetLastError()) + "\n");
        status = BAD;
        return false;
    }
    else
    {
        log.append("connection OK\n");
        status = CONNECTED;
        return true;
    }
}

bool wtr::ServerSocket::SendString(const std::string& str)
{
    if (status != CONNECTED)
    {
        return false;
    }
    int64_t send_bytecount = send(accept_socket, str.c_str(), str.length(), 0);
    if (send_bytecount == SOCKET_ERROR)
    {
        log.append("server-socket send error: " + std::to_string(WSAGetLastError()) + "\n");
        return false;
    }
    else
    {
        log.append("server-socket sent " + std::to_string(send_bytecount) + " bytes");
        return true;
    }
}
bool wtr::ServerSocket::SendFile(const std::filesystem::path& path)
{
    if (status != CONNECTED)
    {
        return false;
    }


    return true;
}
bool wtr::ServerSocket::Receive(std::string& dest)
{
    if (status != CONNECTED)
    {
        return false;
    }

    dest.clear();
    dest.reserve(RECEIVE_BUFFER_LEN);

    int64_t rec_bytecount = recv(accept_socket, dest.data(), RECEIVE_BUFFER_LEN, 0);
    if (rec_bytecount < 0)
    {
        log.append("server-socket receive error: " + std::to_string(WSAGetLastError()) + "\n");
        return false;
    }
    else
    {
        log.append("server-socket received " + std::to_string(rec_bytecount) + " bytes");
        return true;
    }
}

std::string wtr::ServerSocket::getLog() const
{
    return log;
}
wtr::ServerSocket::Status wtr::ServerSocket::getStatus() const
{
    return status;
}

bool wtr::ServerSocket::Close()
{
    if (status != BAD)
    {
        if (server_socket != INVALID_SOCKET)
        {
            closesocket(server_socket);
            server_socket = INVALID_SOCKET;
        }
        if (accept_socket != INVALID_SOCKET)
        {
            closesocket(accept_socket);
            accept_socket = INVALID_SOCKET;
        }
        status = BAD;
        return true;
    }
    return false;
}


wtr::ClientSocket::ClientSocket()
{
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (client_socket == INVALID_SOCKET)
    {
        log.append("socket-initialization failed with error code: " + std::to_string(WSAGetLastError()) + "\n");
    }
    else
    {
        log.append("socket-initialization OK\n");
        status = OPEN;
    }
}

bool wtr::ClientSocket::Connect(const std::string& ip_address, uint64_t port)
{
    if (status != OPEN)
    {
        return false;
    }

    sockaddr_in addr_info = sockaddr_in();
    addr_info.sin_family = AF_INET;
    addr_info.sin_addr.S_un.S_addr = inet_addr(ip_address.c_str());
    addr_info.sin_port = htons(port);

    if (connect(client_socket, reinterpret_cast<sockaddr*>(&addr_info), sizeof(addr_info)) == SOCKET_ERROR)
    {
        log.append("error binding socket: " + std::to_string(WSAGetLastError()) + "\n");
        Close();
        return false;
    }
    else
    {
        this->ip_address = ip_address;
        this->port = port;
        log.append("socket-binding OK\n");
        log.append("client-socket bound to " + ip_address + " at port " + std::to_string(port) + "\n");
    }
}

bool wtr::ClientSocket::Close()
{
    if (status != BAD)
    {
        closesocket(client_socket);
        status = BAD;
        return true;
    }
    return false;
}

bool wtr::ClientSocket::SendString(const std::string& str)
{
    if (status != CONNECTED)
    {
        return false;
    }
    int64_t send_bytecount = send(client_socket, str.c_str(), str.length(), 0);
    if (send_bytecount == SOCKET_ERROR)
    {
        log.append("client-socket send error: " + std::to_string(WSAGetLastError()) + "\n");
        return false;
    }
    else
    {
        log.append("client-socket sent " + std::to_string(send_bytecount) + " bytes");
        return true;
    }
}
bool wtr::ClientSocket::SendFile(const std::filesystem::path& path)
{
    if (status != CONNECTED)
    {
        return false;
    }


    return true;
}
bool wtr::ClientSocket::Receive(std::string& dest)
{
    if (status != CONNECTED)
    {
        return false;
    }

    dest.clear();
    dest.reserve(RECEIVE_BUFFER_LEN);

    int64_t rec_bytecount = recv(client_socket, dest.data(), RECEIVE_BUFFER_LEN, 0);
    if (rec_bytecount < 0)
    {
        log.append("client-socket receive error: " + std::to_string(WSAGetLastError()) + "\n");
        return false;
    }
    else
    {
        log.append("client-socket received " + std::to_string(rec_bytecount) + " bytes");
        return true;
    }
}

std::string wtr::ClientSocket::getLog() const
{
    return log;
}
wtr::ClientSocket::Status wtr::ClientSocket::getStatus() const
{
    return status;
}