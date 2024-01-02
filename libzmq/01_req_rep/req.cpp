
#include <iostream>
#include <string>

#include "zmq.hpp"

// REQ-REP 通信模式示例

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
    
    zmq::context_t context;
    zmq::socket_t socket(context, zmq::socket_type::req);

    socket.connect("tcp://localhost:5555");     // 网络通信
    // socket.connect("ipc:///tmp/req.rep.sock");  // 本地通信

    std::string str1("Hello");
    zmq::message_t msg1(str1.data(), str1.size());
    socket.send(msg1);

    zmq::message_t msg2;
    socket.recv(msg2);

    std::string str2((const char*)msg2.data(), msg2.size());

    std::cout << str2.size() << ": " << str2 << std::endl;
    
    socket.close();
    context.close();

    return 0;
}
