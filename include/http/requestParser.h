#pragma once
#include "http/request.h"
#include "http/parseError.h"

#include <string>
#include <vector>


namespace Http {
    class RequestParser {
    public:
        Http::ParseError parseHeaderAttributes(const std::string& header, std::unordered_map<std::string, std::vector<std::string>>& outHeaderAttributes);
        Http::Request createFullRequest(const std::unordered_map<std::string, std::string>& headerAttributes, const std::string& requestBody);
    };
}

