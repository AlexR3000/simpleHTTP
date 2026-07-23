#pragma once
#include <string>
#include <unordered_map>


namespace Http {
    class Request {
    public:
        std::string method;
        std::string target;
        std::string version;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
    };
};