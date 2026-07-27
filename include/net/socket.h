#pragma once
#include "clientInfo.h"

#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <memory>

namespace Net {
    class Socket {
    public:
        Socket(int addressFamily, int socketType, int protocolType);
        explicit Socket(int socketDescriptor);
        ~Socket();
        Socket(Socket& socket);
        Socket(Socket&& socket);

        int bindSocket(sockaddr_in serverAddress);
        int listenOnSocket(int backlogSize);
        Socket acceptConnection(ClientInfo& clientInfo);
        bool isValid();
        ssize_t receive(void* buffer, size_t bufferSize, int flags);
    private:
        int socket_;
    };
}