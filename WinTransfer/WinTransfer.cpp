#include "WinTransfer.hpp"

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
std::string wtr::getLocalIpv4Address()
{
    char buf[1024];
    if (gethostname(buf, sizeof(buf)) == SOCKET_ERROR)
    {
        throw std::runtime_error("could not determine local hostname");
    }
    
    unsigned char b1 = 0, b2 = 0, b3 = 0, b4 = 0;

    hostent* host = gethostbyname(buf);
    if (!host)
    {
        throw std::runtime_error("could not determine local ip address");
    }

    b1 = reinterpret_cast<in_addr*>(host->h_addr)->S_un.S_un_b.s_b1;
    b2 = reinterpret_cast<in_addr*>(host->h_addr)->S_un.S_un_b.s_b2;
    b3 = reinterpret_cast<in_addr*>(host->h_addr)->S_un.S_un_b.s_b3;
    b4 = reinterpret_cast<in_addr*>(host->h_addr)->S_un.S_un_b.s_b4;

    return std::to_string(b1) + "." + std::to_string(b2) + "." + std::to_string(b3) + "." + std::to_string(b4);
}

wtr::ServerSocket::~ServerSocket()
{
    Close();
}

bool wtr::ServerSocket::Open()
{
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket == INVALID_SOCKET)
    {
        log.append("socket-initialization failed with error code: " + std::to_string(WSAGetLastError()) + "\n");
        return false;
    }
    else
    {
        log.append("socket-initialization OK\n");
        status = OPEN;
        return true;
    }
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
        if (port)
        {
            this->port = port;
        }
        else
        {
            int32_t addrlen = sizeof(addr_info);
            if (getsockname(server_socket, (sockaddr*)&addr_info, &addrlen) == SOCKET_ERROR)
            {
                log.append("error with getting dynamically chosen port: " + std::to_string(WSAGetLastError()) + "\n");
                Close();
                return false;
            }
            else
            {
                int32_t local_port = ntohs(addr_info.sin_port);
                this->port = local_port;
            }
        }
        this->ip_address = ip_address;
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


bool wtr::ServerSocket::Send(const std::string& str)
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
bool wtr::ServerSocket::Receive(std::string& dest, bool large_buf)
{
    if (status != CONNECTED)
    {
        return false;
    }

    int64_t rec_bytecount = NULL;
    dest.clear();

    char* buffer = nullptr;
    if (large_buf)
    {
        buffer = new char[RECEIVE_BUFFER_FILE_LEN];
        std::fill(&buffer[0], &buffer[RECEIVE_BUFFER_FILE_LEN - 1], 0);
        rec_bytecount = recv(accept_socket, buffer, RECEIVE_BUFFER_FILE_LEN, 0);
    }
    else
    {
        buffer = new char[RECEIVE_BUFFER_MSG_LEN];
        std::fill(&buffer[0], &buffer[RECEIVE_BUFFER_MSG_LEN - 1], 0);
        rec_bytecount = recv(accept_socket, buffer, RECEIVE_BUFFER_MSG_LEN, 0);
    }
    if (rec_bytecount < 0)
    {
        log.append("client-socket receive error: " + std::to_string(WSAGetLastError()) + "\n");
        return false;
    }
    else
    {
        log.append("client-socket received " + std::to_string(rec_bytecount) + " bytes");
        dest.append(buffer);
        dest.shrink_to_fit();
        return true;
    }
    if (buffer)
    {
        delete[] buffer;
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
std::string wtr::ServerSocket::getIp() const
{
    return ip_address;
}
uint32_t wtr::ServerSocket::getPort() const
{
    return port;
}


wtr::ClientSocket::~ClientSocket()
{
    Close();
}

bool wtr::ClientSocket::Open()
{
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (client_socket == INVALID_SOCKET)
    {
        log.append("socket-initialization failed with error code: " + std::to_string(WSAGetLastError()) + "\n");
        return false;
    }
    else
    {
        log.append("socket-initialization OK\n");
        status = OPEN;
        return true;
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
        log.append("error connecting socket: " + std::to_string(WSAGetLastError()) + "\n");
        Close();
        return false;
    }
    else
    {
        this->ip_address = ip_address;
        this->port = port;
        log.append("connection OK\n");
        log.append("client-socket bound to " + ip_address + " at port " + std::to_string(port) + "\n");
        status = CONNECTED;
    }
}


bool wtr::ClientSocket::Send(const std::string& str)
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
bool wtr::ClientSocket::Receive(std::string& dest, bool large_buf)
{
    if (status != CONNECTED)
    {
        return false;
    }

    int64_t rec_bytecount = NULL;
    dest.clear();

    char* buffer = nullptr;
    if (large_buf)
    {
        buffer = new char[RECEIVE_BUFFER_FILE_LEN];
        std::fill(&buffer[0], &buffer[RECEIVE_BUFFER_FILE_LEN - 1], 0);
        rec_bytecount = recv(client_socket, buffer, RECEIVE_BUFFER_FILE_LEN, 0);
    }
    else
    {
        buffer = new char[RECEIVE_BUFFER_MSG_LEN];
        std::fill(&buffer[0], &buffer[RECEIVE_BUFFER_MSG_LEN - 1], 0);
        rec_bytecount = recv(client_socket, buffer, RECEIVE_BUFFER_MSG_LEN, 0);
    } 
    if (rec_bytecount < 0)
    {
        log.append("client-socket receive error: " + std::to_string(WSAGetLastError()) + "\n");
        return false;
    }
    else
    {
        log.append("client-socket received " + std::to_string(rec_bytecount) + " bytes");
        dest.append(buffer);
        dest.shrink_to_fit();
        return true;
    }
    if (buffer)
    {
        delete[] buffer;
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
std::string wtr::ClientSocket::getIp() const
{
    return ip_address;
}
uint32_t wtr::ClientSocket::getPort() const
{
    return port;
}