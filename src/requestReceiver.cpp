#include "requestReceiver.h"

#include <string>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>



std::string RequestReceiver::receive() {
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cout << "failed socket" << std::endl;
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


    std::array<char, 5> buffer{};
    std::string receivedMessage = "";
    ssize_t bytesRead = 0;

    std::string rawRequestHeader = "";

    while ((bytesRead = recv(clientSocket, buffer.data(), sizeof(buffer), 0)) > 0) {
        rawRequestHeader.append(buffer.data(), bytesRead);
        if (rawRequestHeader.find(HTTP_TERMINAL_CHUNK) != std::string::npos) {
            std::cout << rawRequestHeader << std::endl;
            break;
        }
    }
    std::cout << "received request" << std::endl;

    return rawRequestHeader;
}