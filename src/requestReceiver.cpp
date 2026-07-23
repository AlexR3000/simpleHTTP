#include "requestReceiver.h"
#include "http/request.h"

#include <string>
#include <iostream>
#include <array>
#include <exception>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include <map>
#include <http/requestParser.h>


// alex TODO refactor all in one class function
Http::Request RequestReceiver::receive() {
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cout << "failed to create socket" << std::endl;
    }

    // Using clang-tidy and c-style sockets are a bit tricky. C-style casts are not allowed
    // but neither is reinterpret_cast
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    int bound = bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if (bound < 0) {
        std::cout << "bind failed" << std::endl;
    }

    int listened = listen(serverSocket, 5);
    if (listened < 0) {
        std::cout << "failed to listen" << std::endl;
    }
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cout << "failed client" << std::endl;
    }


    std::array<char, 1024> buffer{};

    ssize_t bytesRead = 0;
    std::string rawRequestHeader = "";
    std::string rawRequestBody = "";

    // Read the header
    while ((bytesRead = recv(clientSocket, buffer.data(), buffer.size(), 0)) > 0) {
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
    std::unordered_map<std::string, std::vector<std::string>>  requestAttributes{};
    Http::ParseError error = parser.parseHeaderAttributes(rawRequestHeader, requestAttributes);

    // alex TODO proper error handling for possible parser errors. 
    if (error != Http::ParseError::None) {
        throw std::runtime_error("Failed to parse Http header");
    }

    size_t contentLength = 0;
    if (requestAttributes.count("Content-Length") && !requestAttributes["Content-Length"].empty()) {
        contentLength = std::stoi(requestAttributes["Content-Length"].front());
    }


    // Read the body, if required
    while (rawRequestBody.length() < contentLength) {
        bytesRead = recv(clientSocket, buffer.data(), buffer.size(), 0);
        if (bytesRead <= 0) {
            break;
        }

        rawRequestBody.append(buffer.data(), bytesRead);
        if (rawRequestBody.length() >= contentLength) {
            break;
        }
    }

    std::cout << "request received" << std::endl;
    std::cout << rawRequestHeader << std::endl;

    close(clientSocket);
    close(serverSocket);    

    return Http::Request();
}