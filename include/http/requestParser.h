#pragma once
#include "request.h"
#include "parseError.h"
#include "methods.h"

#include <string>
#include <vector>



namespace Http {
    class RequestParser {
    public:
        Http::ParseError parseHeaderAttributes(const std::string_view header, Request& outRequest);
    private:
        const std::string_view httpLineSplitToken_ = "\r\n";
        Http::ParseError parseFirstRequestLine(const std::string_view firstLine, Request& outRequest);
        Http::ParseError insertKeyValue(const std::string_view key, const std::string_view value, Request& outRequest);
        Http::Method parseMethod(std::string_view method);
    };
}

