#include "net/socket.h"
#include "net/clientInfo.h"

#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>


Net::Socket::Socket(int addressFamily, int socketType, int protocolType): socket_(socket(addressFamily, socketType, protocolType)) {
    if (socket_ < 0) {
        throw std::system_error(errno, std::system_category(), "Failed to create socket");
    }
}

Net::Socket::Socket(int socketDescriptor): socket_(socketDescriptor) {
    if (socket_ < 0) {
        throw std::system_error(errno, std::system_category(), "Failed to create socket");
    }
}

Net::Socket::~Socket() {
    close(socket_);
}


int Net::Socket::bindSocket(sockaddr_in serverAddress) {
    // Using clang-tidy and c-style sockets are a bit tricky. C-style casts are not allowed
    // but neither is reinterpret_cast
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return bind(socket_, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    
}


int Net::Socket::listenOnSocket(int backlogSize) {
    return listen(socket_, backlogSize);
}


Net::Socket Net::Socket::acceptConnection(ClientInfo& clientInfo) {
    // Using clang-tidy and c-style sockets are a bit tricky. C-style casts are not allowed
    // but neither is reinterpret_cast
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return Net::Socket(accept(socket_, reinterpret_cast<sockaddr*>(&clientInfo.clientAddress), &clientInfo.clientAddressLength));
}


ssize_t Net::Socket::receive(void* buffer, size_t bufferSize, int flags) {
    return recv(socket_, buffer, bufferSize, flags);
}


bool Net::Socket::isValid() {
    return socket_ < 0 ? false : true;
}