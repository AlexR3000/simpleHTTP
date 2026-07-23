#include "requestReceiver.h"

#include <iostream>


int main() {
    std::cout << "running" << std::endl;
    RequestReceiver receiver;
    Http::Request request = receiver.receive();
}