#include "requestReceiver.h"
#include "net/tcpListener.h"
#include "net/clientInfo.h"

#include <iostream>


int main() {
    std::cout << "running" << std::endl;

    // a TODO read from settings or commandline
    in_port_t port = 8080;
    std::string_view inAddr = "0.0.0.0";


    Net::TcpListener listener(port, inAddr);
    int listenResult = listener.Tcplisten();
    if (listenResult < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }

    ClientInfo clientInfo{};
    Net::Socket client = listener.TcpAcceptClient(clientInfo);
    if (!client.isValid()) {
        throw std::runtime_error("Failed to accept client");
    }    

    RequestReceiver receiver;
    Http::Request request = receiver.receive(client);
}