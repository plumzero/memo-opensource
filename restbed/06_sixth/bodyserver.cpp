
#include <stdio.h>
#include <string>
#include <memory>
#include <iostream>
#include <restbed>

#include "header.h"

void post_method_handler(const std::shared_ptr<restbed::Session> session)
{
    const auto& request = session->get_request();
    
    if (request->has_header("Content-Length")) {
        int clen = request->get_header("Content-Length", 0);
        
        session->fetch(clen, [&request](const std::shared_ptr<restbed::Session> session,
                                        const restbed::Bytes& body) 
        {

            std::string recvs(body.begin(), body.end());
            std::cout << "size=" << recvs.size() << ", recv: " << bin2hex(recvs) << std::endl;
                
            const std::string body2 = "sever had received";
                
            session->yield(restbed::OK, body2, { { "Content-Length", std::to_string(body2.size()) },
                                                { "Connection", "keep-alive" } } );
        
        });
    } else {
        std::cout << "no Content-Length" << std::endl;
    }

}

int main( const int, const char** )
{
    auto resource = std::make_shared<restbed::Resource>();
    resource->set_path("/resource/{name: .*}");
    resource->set_method_handler("POST", post_method_handler);
    
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(1984);
    
    restbed::Service service;
    service.publish(resource);
    service.start(settings);
    
    return 0;
}
