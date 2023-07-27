
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>
#include <restbed>

using thread_ptr = std::shared_ptr<std::thread>;

void post_method_handler_one(const std::shared_ptr<restbed::Session> session)
{
    const auto& request = session->get_request();
    
    std::cout << "color=" << request->get_header("Color")
              << "index=" << request->get_header("Index", 0) << std::endl;
        
    const std::string body{ '\x00', '\x00', '\x00', '\x00', '\x00', '\x02',
                            '\x00', '\x00', '\x00', '\x00', '\x00', '\x00' };
        
    session->yield(restbed::OK, body, { { "Content-Length", std::to_string(body.size()) }} );
}

void post_method_handler_two(const std::shared_ptr<restbed::Session> session)
{
    const auto& request = session->get_request();
    
    std::cout << "color=" << request->get_header("Color")
              << "index=" << request->get_header("Index", 0) << std::endl;
    
    const std::string body{ '\x50', '\x20', '\x04', '\x00', '\x00', '\x00',
                            '\x00', '\x00', '\x91', '\x06', '\x00', '\x00',
                            '\x50', '\x20', '\x04', '\x00', '\x00', '\x00' };
        
    session->yield(restbed::OK, body, { { "Content-Length", std::to_string(body.size()) }} );
}

void post_method_handler_thr(const std::shared_ptr<restbed::Session> session)
{
    const auto& request = session->get_request();
    
    std::cout << "color=" << request->get_header("Color")
              << "index=" << request->get_header("Index", 0) << std::endl;
    
    const std::string body{ '\x00', '\x00', '\x00', '\x00', '\x00', '\x02',
                            '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
                            '\x50', '\x20', '\x04', '\x00', '\x00', '\x00',
                            '\x00', '\x00', '\x91', '\x06', '\x00', '\x00',
                            '\x00', '\x00', '\x00', '\x00', '\x00', '\x23'};
        
    session->yield(restbed::OK, body, { { "Content-Length", std::to_string(body.size()) }} );
}

int main( const int, const char** )
{
    auto resource_one = std::make_shared<restbed::Resource>();
    resource_one->set_path("/api/v2/color/red");
    resource_one->set_method_handler("POST", post_method_handler_one);
    
    auto resource_two = std::make_shared<restbed::Resource>();
    resource_two->set_path("/api/v2/color/green");
    resource_two->set_method_handler("POST", post_method_handler_two);
        
    auto resource_thr = std::make_shared<restbed::Resource>();
    resource_thr->set_path("/api/v2/color/blue");
    resource_thr->set_method_handler("POST", post_method_handler_thr);

    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(1984);
    settings->set_default_header("Connection", "keep-alive");
    
    restbed::Service service;
    service.publish(resource_one);
    service.publish(resource_two);
    service.publish(resource_thr);
    service.start(settings);

    return 0;

    // std::vector<thread_ptr> threads;
    
    // int i = 0;
    // for (i = 0; i < 1; i++) {
        // threads.push_back(std::make_shared<std::thread>([&resource, &resource2](std::shared_ptr<restbed::Settings>& set){
            // restbed::Service service;
            // service.publish(resource);
            // service.publish(resource2);
            // service.start(set);
        // }, std::ref(settings)));
    // }
    
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    
    return 0;
}
