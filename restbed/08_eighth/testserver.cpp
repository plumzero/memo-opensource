
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <restbed>

void get_method_handler_red(const std::shared_ptr<restbed::Session> session)
{
    const auto& request = session->get_request();
    
    const std::string body{ "The flowers is red.\n" };

    session->yield(restbed::OK, body, { { "Content-Length", std::to_string(body.size()) }} );
}

void post_method_handler_green(const std::shared_ptr<restbed::Session> session)
{
    const auto& request = session->get_request();
    
    std::cout << "color=" << request->get_header("Color")
              << "index=" << request->get_header("Index", 0) << std::endl;
    
    const std::string body = std::string("Got the index ") + std::to_string(request->get_header("Index", 0)) + ", and the water is green.\n";
        
    session->yield(restbed::OK, body, { { "Content-Length", std::to_string(body.size()) }} );
}

void post_method_handler_blue(const std::shared_ptr<restbed::Session> session)
{
    const auto& request = session->get_request();
    
    if (request->has_header("Content-Length")) {
        int clen = request->get_header("Content-Length", 0);
        
        session->fetch(clen, [&request](const std::shared_ptr<restbed::Session> session,
                                        const restbed::Bytes& body) 
        {

            std::string recvs(body.begin(), body.end());
            std::cout << "size=" << recvs.size() << ", recv: " << recvs << std::endl;
                
            const std::string body2 = "sever had received string '" + recvs + "', and the sky is blue.\n";
                
            session->yield(restbed::OK, body2, { { "Content-Length", std::to_string(body2.size()) },
                                                { "Connection", "keep-alive" } } );
        
        });
    } else {
        std::cout << "no Content-Length" << std::endl;
    }
}

void post_method_handler_uriparams(const std::shared_ptr<restbed::Session> session)
{
    const auto& request = session->get_request();
    
    std::cout << "color=" << request->get_header("Color")
              << "index=" << request->get_header("Index", 0) << std::endl;
    
    const std::string body = std::string("Got the index ") + std::to_string(request->get_header("Index", 0)) + ", and have params.\n";
        
    session->yield(restbed::OK, body, { { "Content-Length", std::to_string(body.size()) }} );
}


int main( const int, const char** )
{
    auto resource_red = std::make_shared<restbed::Resource>();
    resource_red->set_path("/api/v2/red");
    resource_red->set_method_handler("GET", get_method_handler_red);
    
    auto resource_green = std::make_shared<restbed::Resource>();
    resource_green->set_path("/api/v2/green");
    resource_green->set_method_handler("POST", post_method_handler_green);
        
    auto resource_blue = std::make_shared<restbed::Resource>();
    resource_blue->set_path("/api/v2/blue");
    resource_blue->set_method_handler("POST", post_method_handler_blue);

    auto resource_params = std::make_shared<restbed::Resource>();
    resource_params->set_path("/forward/exception/add?level=2&title=timeMarket&code=401&detail=timeMarket%20error");
    resource_params->set_method_handler("POST", post_method_handler_uriparams);

    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(1984);
    settings->set_default_header("Connection", "keep-alive");
    
    restbed::Service service;
    service.publish(resource_red);
    service.publish(resource_green);
    service.publish(resource_blue);
    service.publish(resource_params);
    service.start(settings);
    
    return 0;
}
