
#include <memory>
#include <future>
#include <cstdio>
#include <cstdlib>
#include <restbed>

#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include "header.h"

int main(const int, const char**)
{
    auto request = std::make_shared<restbed::Request>(
                restbed::Uri("http://127.0.0.1:1984/resource/fangyi"));
    request->set_header("Connection", "keep-alive");
    request->set_method("POST");

    int i;
    for (i = 0; i < 2000; i++) {
        request->set_header("MyName", "shencheng");
        request->set_header("Index", std::to_string(i));
        
        auto response = restbed::Http::sync(request);
        if (response->get_status_code() == 200) {
            auto clen = response->get_header("Content-Length", 0);
            /** std::is_same<bytes, std::vector<char>>::value = true */
            auto bytes = restbed::Http::fetch(clen, response);
            
            std::cout << "Content-Length: " << clen << " | ";
            for (const auto & b : bytes) {
                printf("%02X", static_cast<int>((unsigned char)b));
            }
            std::cout << std::endl;            
        }
    }
        
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
