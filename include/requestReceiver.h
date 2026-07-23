#pragma once

#include "http/request.h"

#include <string>


class RequestReceiver {
public:
    const std::string HTTP_TERMINAL_CHUNK = "\r\n\r\n";
    Http::Request receive();
};