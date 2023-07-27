
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "zmq.hpp"

// REQ-REP 通信模式示例

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    
    zmq::context_t context;
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");                 // 网络通信
    // socket.bind("ipc:///tmp/req.rep.sock");   // 本地通信
    
    zmq::message_t msg1;
    socket.recv(msg1);

    std::string str1((const char*)msg1.data(), msg1.size());
    std::cout << str1.size() << ": " << str1 << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::string str2 = str1 + " World";
    zmq::message_t msg2(str2.data(), str2.size());

    socket.send(msg2);
    
    socket.close();
    context.close();

    return 0;
}
