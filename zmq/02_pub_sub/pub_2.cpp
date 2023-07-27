
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "zmq.hpp"

// PUB-SUB 模式
// PUB 作为服务端

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

    // 第一个消息
    zmq::message_t msg_cno_1(cno_a.c_str(), cno_a.size());
    std::string str1("Hello World");
    zmq::message_t msg1(str1.data(), str1.size());
    socket.send(msg_cno_1, zmq::send_flags::sndmore);
    socket.send(msg1, zmq::send_flags::dontwait);

    // 第二个消息
    zmq::message_t msg_cno_2(cno_b.c_str(), cno_b.size());
    std::string str2("Shit Shit!");
    zmq::message_t msg2(str2.data(), str2.size());
    socket.send(msg_cno_2, zmq::send_flags::sndmore);
    socket.send(msg2, zmq::send_flags::dontwait);

    // 第三个消息
    zmq::message_t msg_cno_3(cno_a.c_str(), cno_a.size());
    std::string str3("Hi Earth");
    zmq::message_t msg3(str3.data(), str3.size());
    socket.send(msg_cno_3, zmq::send_flags::sndmore);
    socket.send(msg3, zmq::send_flags::dontwait);

    std::cout << "over" << std::endl;

    return 0;
}
