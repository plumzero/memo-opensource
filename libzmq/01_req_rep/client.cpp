
#include <string>
#include <stdio.h>

#include "zmq.hpp"

#include "_type.h"

int main(int argc, char* argv[])
{
  (void) argc;
  (void) argv;
  
  zmq::context_t context;
  zmq::socket_t socket(context, zmq::socket_type::req);

  socket.connect("tcp://localhost:5555");
  
  {
    Payload_t pt;
    pt.id = 10001;
    pt.payload = std::vector<uint8_t>({0x00, 0x01, 0x02, 0x03, 0x02, 0x01, 0x00}); 

    zmq::message_t id(&pt.id, sizeof(pt.id));
    socket.send(id, zmq::send_flags::sndmore);
    zmq::message_t payload(pt.payload.data(), pt.payload.data() + pt.payload.size());
    socket.send(payload, zmq::send_flags::dontwait);

    zmq::message_t retmsg;
    socket.recv(retmsg);
    std::string s((const char*)retmsg.data(), retmsg.size());

    printf("ret=%s\n", s.c_str());
  }

  socket.close();
  context.close();

  return 0;
}
