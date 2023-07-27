
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
    
    time_t bt, et;
    
    int i, total = 20000, counter = 0;
    if (total < 5000) {
        printf("number of times for test too little\n");
        return -1;
    }
    
    time(&bt);
    for (i = 0; i < total; i++) {
        request->set_header("MyName", "shencheng");
        request->set_header("Index", std::to_string(i));
        
        auto response = restbed::Http::sync(request);
        if (response->get_status_code() == 200) {
            auto clen = response->get_header("Content-Length", 0);
            auto bytes = restbed::Http::fetch(clen, response);
            
            if ((size_t)clen == bytes.size()) {
                counter++;
            }           
        }
    }
    time(&et);
    int secs = (int)(et - bt);
    
    printf("%d request&response consumed %d seconds, tps=%.3f\n", total, secs, (double)total / secs);
            
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
