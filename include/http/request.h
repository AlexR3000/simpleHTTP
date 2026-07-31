#pragma once
#include "methods.h"

#include <string>
#include <unordered_map>
#include <vector>



namespace Http {
    using AttributeMap = std::unordered_map<std::string, std::vector<std::string>>;
    class Request {
    public:
        Method method;
        std::string url;
        std::string version;
        size_t contentLength;
        AttributeMap headers;
        std::string body;
    };
};