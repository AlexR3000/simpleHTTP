#include "requestReceiver.h"
#include "http/request.h"
#include "http/requestParser.h"
#include "net/socket.h"

#include <string>
#include <iostream>
#include <array>
#include <exception>


Http::Request RequestReceiver::receive(Net::Socket& receiverSocket) {
    std::cout << "receiving" << std::endl;
    std::array<char, RECEIVE_BUFFER_SIZE> buffer{};

    ssize_t bytesRead = 0;
    std::string rawRequestHeader = "";
    std::string rawRequestBody = "";
    int flags = 0;

    // Read the header
    while ((bytesRead = receiverSocket.receive(buffer.data(), buffer.size(), flags)) > 0) {
        rawRequestHeader.append(buffer.data(), bytesRead);
        auto terminalPosition = rawRequestHeader.find(HTTP_TERMINAL_CHUNK);

        // copies body bytes into the body string before removing them from the header string if first body bytes have been received.
        if (terminalPosition != std::string::npos) {
            rawRequestBody = rawRequestHeader.substr(terminalPosition + HTTP_TERMINAL_CHUNK.size());
            rawRequestHeader = rawRequestHeader.substr(0, terminalPosition);
            break;
        }
    }

    
    Http::RequestParser parser{};
    Http::Request request{};
    Http::ParseError error = parser.parseHeaderAttributes(rawRequestHeader, request);

    // alex TODO proper error handling for possible parser errors. 
    if (error != Http::ParseError::None) {
        std::cout << static_cast<int>(error) << std::endl;
        throw std::runtime_error("Failed to parse Http header");
    }



    // Read the body, if required
    while (rawRequestBody.length() < request.contentLength) {
        bytesRead = receiverSocket.receive(buffer.data(), buffer.size(), 0);
        if (bytesRead <= 0) {
            break;
        }

        rawRequestBody.append(buffer.data(), bytesRead);
        if (rawRequestBody.length() >= request.contentLength) {
            break;
        }
    }

    request.body = rawRequestBody;

    return request;
}