
#include <stdio.h>
#include <string>
#include <memory>
#include <iostream>
#include <restbed>

#include "header.h"

void post_method_handler(const std::shared_ptr<restbed::Session> session)
{
    const auto& request = session->get_request();
    
    unsigned char buf[32]{0};
    randomX4(buf, 32);
    
    const std::string body = std::string(buf, buf + 32);
        
    session->yield(restbed::OK, body, { { "Content-Length", std::to_string(body.size()) },
                                        { "Connection", "keep-alive" } } );
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
