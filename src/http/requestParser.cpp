#include "http/requestParser.h"
#include "http/request.h"
#include "http/parseError.h"
#include "http/methods.h"

#include <string>
#include <sstream>
#include <ranges>

#include <iostream>
#include <algorithm>



Http::ParseError Http::RequestParser::parseHeaderAttributes(const std::string_view header, Request& outRequest) {
    outRequest.headers.clear();


    ParseError error = parseFirstRequestLine(header, outRequest);
    if (error != ParseError::None) {
        return error;
    }

    for (auto&& attributeValuePair : header.substr(header.find(httpLineSplitToken_)) | std::views::split(httpLineSplitToken_)) {
        std::string token(attributeValuePair.begin(), attributeValuePair.end());
        if (token.empty()) {
            continue;
        }

        auto pos = token.find(':');
        if (pos != std::string::npos) {
            std::string key = token.substr(0, pos);
            std::transform(
                key.begin(),
                key.end(),
                key.begin(),
                [](unsigned char c) { return std::tolower(c); }
            );

            std::string value = token.substr(pos + 1);

            error = insertKeyValue(key, value, outRequest);
            if (error != ParseError::None) {
                return error;
            }            
        }
        else {
            return ParseError::InvalidAttribute;
        }
    }

    return ParseError::None;
}


Http::ParseError Http::RequestParser::parseFirstRequestLine(const std::string_view header, Request& outRequest) {
    auto methodLineEnd = header.find(httpLineSplitToken_);
    if (methodLineEnd == std::string::npos) {
        return ParseError::InvalidRequest;
    }
    auto firstLine = header.substr(0, methodLineEnd);

    
    auto methodSeparatorPosition = firstLine.find(' ');
    if (methodSeparatorPosition == std::string::npos) {
        return ParseError::InvalidRequest;
    }

    outRequest.method = parseMethod(firstLine.substr(0, methodSeparatorPosition));

    
    auto urlSeparatorPosition = firstLine.find(' ', methodSeparatorPosition + 1);
    if (urlSeparatorPosition == std::string::npos) {
        return ParseError::InvalidRequest;
    }
    outRequest.url = firstLine.substr(methodSeparatorPosition + 1, urlSeparatorPosition - methodSeparatorPosition - 1);
    outRequest.version = firstLine.substr(urlSeparatorPosition + 1);


    return ParseError::None;
}


Http::ParseError Http::RequestParser::insertKeyValue(const std::string_view key, const std::string_view value, Request& outRequest) {
    if (key == "content-length") {
        try {
            outRequest.contentLength = static_cast<size_t>(std::stoull(std::string(value)));
        }
        catch (std::exception e) {
            std::cout << "Error for attribute content-length: " << e.what() << std::endl;
            return ParseError::InvalidAttribute;
        }
    }
    else {
        // on else the key and value are inserted into the generic header attribute map of the request object
        // to handle everything I don't handle specifically
        outRequest.headers[std::string(key)].push_back(std::string(value));
    }

    return ParseError::None;
}


Http::Method Http::RequestParser::parseMethod(std::string_view method)
{
    if (method == "GET")     return Http::Method::GET;
    if (method == "POST")    return Http::Method::POST;
    if (method == "PUT")     return Http::Method::PUT;
    if (method == "DELETE")  return Http::Method::DELETE;
    if (method == "HEAD")    return Http::Method::HEAD;
    if (method == "OPTIONS") return Http::Method::OPTIONS;

    return Http::Method::UNKNOWN;
}