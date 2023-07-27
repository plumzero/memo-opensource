
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "zmq.hpp"

// PUSH-PULL 模式

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::push);

    socket.bind("tcp://*:5555");

    // 为了测试: 等待网络建立
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Hello 100 次
    for (int i = 0; i < 100; i++) {
        std::string str(std::to_string(i) + " Hello");
        zmq::message_t msg(str.data(), str.size());
        socket.send(msg);
    }

    std::cout << "send over" << std::endl;

    return 0;
}
