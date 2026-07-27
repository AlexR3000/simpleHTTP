#pragma once
#include <sys/socket.h>
#include <netinet/in.h>


struct ClientInfo {
    sockaddr_in clientAddress;
    socklen_t clientAddressLength;
};