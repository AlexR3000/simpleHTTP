#include "net/tcpListener.h"
#include "net/clientInfo.h"
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>


Net::TcpListener::TcpListener(uint16_t port, std::string_view inAddress): socket_(ADDRESS_FAMILY, SOCKET_TYPE, PROTOCOL_TCP), serverAddress_() {
    serverAddress_.sin_family = ADDRESS_FAMILY;
    serverAddress_.sin_port = htons(port);
    int addressError = inet_pton(ADDRESS_FAMILY, inAddress.data(), &serverAddress_.sin_addr);
    if (addressError < 0) {
        throw std::runtime_error("Given Address is invalid:" + std::string(inAddress));
    }

    int bindError = socket_.bindSocket(serverAddress_);
    if (bindError < 0) {
        throw std::system_error(errno, std::system_category(), "Failed to bind the socket");
    }
}


int Net::TcpListener::Tcplisten() {
    int listened = socket_.listenOnSocket(BACKLOG_SIZE);
    return listened;
}


Net::Socket Net::TcpListener::TcpAcceptClient(ClientInfo& clientInfo) {
    return socket_.acceptConnection(clientInfo);
}