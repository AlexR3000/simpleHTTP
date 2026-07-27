#pragma once
#include "http/request.h"
#include "net/socket.h"
#include <string_view>


class RequestReceiver {
public:
    Http::Request receive(Net::Socket& receiverSocket);
private:
    static constexpr std::string_view HTTP_TERMINAL_CHUNK = "\r\n\r\n";
    static constexpr size_t RECEIVE_BUFFER_SIZE = 1024;
};