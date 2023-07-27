
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
    for (i = 0; i < 3; i++) {
        std::string body = "{ \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] }";
        std::cout << "send: " << body << std::endl;
        request->set_body(body);
        request->set_header("Content-Length", std::to_string(body.size()));
        // request->set_header("Content-Type", "application/json");
        auto response = restbed::Http::sync(request);
        if (response->get_status_code() == 200) {
            auto clen = response->get_header("Content-Length", 0);
            /** std::is_same<bytes, std::vector<char>>::value = true */
            auto bytes = restbed::Http::fetch(clen, response);
            
            std::string recvs = std::string(bytes.begin(), bytes.end());
            
            std::cout << "Content-Length: " << clen << " | " << recvs << std::endl;
        }
    }
        
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
