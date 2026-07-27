#pragma once
#include "net/socket.h"

#include <string_view>

#include <stdint.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

namespace Net {
    class TcpListener {
    public:
        TcpListener(uint16_t port, std::string_view inAddress);
        int Tcplisten();
        Net::Socket TcpAcceptClient(ClientInfo& clientInfo);
    private:
        Socket socket_;
        sockaddr_in serverAddress_;
        static constexpr int BACKLOG_SIZE = 5;
        static constexpr sa_family_t ADDRESS_FAMILY = AF_INET;
        static constexpr int SOCKET_TYPE = SOCK_STREAM;
        static constexpr int PROTOCOL_TCP = 0;
    };
}