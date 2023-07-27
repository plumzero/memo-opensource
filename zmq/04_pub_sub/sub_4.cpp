
#include <iostream>
#include <string>

#include "zmq.hpp"

// PUB-SUB 模式
// 尝试发送或接收数据包时合并的可行性

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::sub);

    socket.connect("tcp://localhost:5555");

    std::string cno_a("friendly");
    socket.setsockopt(ZMQ_SUBSCRIBE, cno_a.c_str(), cno_a.size());

    while (true) {
        zmq::message_t channel;
        socket.recv(channel, zmq::recv_flags::none);
        std::cout << "[" << std::string((const char*)channel.data(), channel.size()) << "]" << std::endl;

        zmq::message_t msg;
        socket.recv(msg, zmq::recv_flags::none);
        
        std::cout << "{" << std::string((const char*)msg.data(), msg.size()) << "}" << std::endl;
    }
    
    return 0;
}
