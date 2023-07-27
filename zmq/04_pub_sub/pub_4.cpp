
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "zmq.hpp"

// PUB-SUB 模式
// 尝试发送或接收数据包时合并的可行性

// 合并消息发送是可行的，不过在发送消息时，切不要将标志位置为 zmq::send_flags::sndmore, 否则解包有问题
// 具体效率如何还需进一步测试

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::pub);

    socket.bind("tcp://*:5555");

    // 为了测试: 等待客户端的连接
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 两种消息
    std::string cno_a("friendly");
    std::string cno_b("unfriendly");

    {
        std::string str(cno_a + "Hello World");
        zmq::message_t msg(str.c_str(), str.size());
        socket.send(msg);
    }

    {
        std::string str(cno_b + "Shit Shit" + cno_a + "Good Morning");
        zmq::message_t msg(str.c_str(), str.size());
        socket.send(msg);
    }

    {
        std::string str(cno_a + "Hi Earth" + cno_b + "What Fuck");
        zmq::message_t msg(str.c_str(), str.size());
        socket.send(msg);
    }

    std::cout << "over" << std::endl;

    return 0;
}
