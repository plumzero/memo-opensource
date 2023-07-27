/* 
   Mathieu Stefani, 13 février 2016
   
   Example of an hello world server
*/


#include <pistache/endpoint.h>

class HelloHandler : public Pistache::Http::Handler {
public:

    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
        response.send(Pistache::Http::Code::Ok, "Hello World");
    }
};

int main() {
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));
    auto opts = Pistache::Http::Endpoint::options()
        .threads(1);

    Pistache::Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<HelloHandler>());
    server.serve();

    server.shutdown();
    
    return 0;
}
