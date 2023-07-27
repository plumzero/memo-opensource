
#include <iostream>
#include <string>
#include <vector>

#include "zmq.hpp"

// PUSH-PULL 模式

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    
    zmq::context_t context(1);

    zmq::socket_t recv_socket(context, zmq::socket_type::pull);
    recv_socket.connect("tcp://localhost:5555");

    zmq::socket_t send_socket(context, zmq::socket_type::push);
    send_socket.connect("tcp://localhost:5556");

    // 一个处理线程
    while (true) {
        zmq::message_t rmsg;
        recv_socket.recv(rmsg);
        std::cout << std::string((const char*)rmsg.data(), rmsg.size()) << std::endl;
        // 处理一下发送
        std::string str = std::string((const char*)rmsg.data(), rmsg.size()) + " World";
        zmq::message_t smsg(str.data(), str.size());
        send_socket.send(smsg);
    }

    return 0;
}
