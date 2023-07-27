
#include <iostream>
#include <string>

#include "zmq.hpp"

// PUSH-PULL 模式

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::pull);

    socket.bind("tcp://*:5556");

    while (true) {
        zmq::message_t msg;
        socket.recv(msg);
        std::string str((const char*)msg.data(), msg.size());
        std::cout << str << std::endl;
    }

    return 0;
}
