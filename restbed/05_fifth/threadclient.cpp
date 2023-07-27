
#include <memory>
#include <future>
#include <cstdio>
#include <cstdlib>
#include <restbed>

#include <string>
#include <iostream>
#include <thread>
#include <chrono>

int main(const int, const char**)
{
    auto request_one = std::make_shared<restbed::Request>(
                restbed::Uri("http://127.0.0.1:1984/api/v2/color/red"));
    request_one->set_header("Connection", "keep-alive");
    request_one->set_method("POST");
    
    auto request_two = std::make_shared<restbed::Request>(
                restbed::Uri("http://127.0.0.1:1985/api/v2/color/green"));
    request_two->set_header("Connection", "keep-alive");
    request_two->set_method("POST");

    auto request_thr = std::make_shared<restbed::Request>(
                restbed::Uri("http://127.0.0.1:1986/api/v2/color/blue"));
    request_thr->set_header("Connection", "keep-alive");
    request_thr->set_method("POST");

    int i, total = 3000, counter1 = 0, counter2 = 0, counter3 = 0;
    for (i = 0; i < total; i++) {
        request_one->set_header("Color", "Red");
        request_one->set_header("Index", std::to_string(i));
        
        auto response_one = restbed::Http::sync(request_one);
        if (response_one->get_status_code() == 200) {
            auto clen = response_one->get_header("Content-Length", 0);
            auto bytes = restbed::Http::fetch(clen, response_one);
            
            if ((size_t)clen == bytes.size()) {
                std::cout << "Content-Length: " << clen << " | ";
                for (const auto & b : bytes) {
                    printf("%02X", static_cast<int>((unsigned char)b));
                }
                std::cout << std::endl;
                
                counter1++;
            }
        }
        
        request_two->set_header("Color", "Green");
        request_two->set_header("Index", std::to_string(i + 100));
        auto response_two = restbed::Http::sync(request_two);
        if (response_two->get_status_code() == 200) {
            auto clen = response_two->get_header("Content-Length", 0);
            auto bytes = restbed::Http::fetch(clen, response_two);

            if ((size_t)clen == bytes.size()) {
                std::cout << "Content-Length: " << clen << " | ";
                for (const auto & b : bytes) {
                    printf("%02X", static_cast<int>((unsigned char)b));
                }
                std::cout << std::endl;
                
                counter2++;
            }
        }
        
        request_thr->set_header("Color", "Blue");
        request_thr->set_header("Index", std::to_string(i + 1000));
        auto response_thr = restbed::Http::sync(request_thr);
        if (response_thr->get_status_code() == 200) {
            auto clen = response_thr->get_header("Content-Length", 0);
            auto bytes = restbed::Http::fetch(clen, response_thr);

            if ((size_t)clen == bytes.size()) {
                std::cout << "Content-Length: " << clen << " | ";
                for (const auto & b : bytes) {
                    printf("%02X", static_cast<int>((unsigned char)b));
                }
                std::cout << std::endl;
                
                counter3++;
            }
        }       
    }
    
    std::cout << "total=" << total << std::endl;
    std::cout << "counter1=" << counter1 << std::endl;
    std::cout << "counter2=" << counter2 << std::endl;
    std::cout << "counter3=" << counter3 << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
