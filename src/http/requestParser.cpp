#include "http/requestParser.h"
#include "http/request.h"
#include "http/parseError.h"

#include <string>
#include <ranges>

using AttributeMap = std::unordered_map<std::string, std::vector<std::string>>;

 Http::ParseError Http::RequestParser::parseHeaderAttributes(const std::string& header, AttributeMap& outHeaderAttributes) {
     outHeaderAttributes.clear();
     ParseError error = ParseError::None;
     

    for (auto&& attributeValuePair : header | std::views::split(std::string_view{ "\r\n" })) {
        std::string token(attributeValuePair.begin(), attributeValuePair.end());

        auto pos = token.find(':');
        if (pos != std::string::npos) {
            outHeaderAttributes[token.substr(0, pos)].push_back(token.substr(pos + 1));
        }
        else {
            
        }
    }

    return error;
}