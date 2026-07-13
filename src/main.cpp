
#include "project.h"
#include "requestReceiver.h"

#include <iostream>


int main() {

    Project p;
    std::cout << p.someFunction() << " running test" << std::endl;


    RequestReceiver receiver;
    receiver.receive();


}