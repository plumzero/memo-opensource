
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "zmq.hpp"

#include "_type.h"

int main(int argc, char* argv[])
{
  (void) argc;
  (void) argv;
  
  zmq::context_t context;
  zmq::socket_t socket(context, zmq::socket_type::rep);
  socket.bind("tcp://*:5555");
  
  while (true)
  {
    printf("before recv\n");
    // 消息类型
    zmq::message_t idmsg;
    socket.recv(idmsg, zmq::recv_flags::none);
    int id;
    memcpy(&id, idmsg.data(), sizeof(int));
    std::cout << "id = " << id << std::endl;
    // 处理对应消息
    zmq::message_t payloadmsg;
    socket.recv(payloadmsg, zmq::recv_flags::none);
    std::vector<uint8_t> payload(payloadmsg.size());
    memcpy(payload.data(), payloadmsg.data(), payloadmsg.size());
    for (size_t i = 0; i < payload.size(); i++) {
      printf("%02X ", (int)static_cast<uint8_t>(payload[i]));
    }
    printf("\n");
    // 返回值
    std::string s("ok");
    zmq::message_t retmsg(s.data(), s.size());
    socket.send(retmsg);
  }
  
  socket.close();
  context.close();

  return 0;
}
