
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
    // test pair: Red
    // test goal: GET
    // test cmd : curl http://127.0.0.1:1984/api/v2/red
    auto request_red = std::make_shared<restbed::Request>(
                restbed::Uri("http://127.0.0.1:1984/api/v2/red"));
    request_red->set_header("Connection", "keep-alive");
    request_red->set_method("GET");

    auto response_red = restbed::Http::sync(request_red);
    if (response_red->get_status_code() == 200) {
        auto clen = response_red->get_header("Content-Length", 0);
        auto bytes = restbed::Http::fetch(clen, response_red);
        std::string s(bytes.begin(), bytes.end());
        std::cout << s << std::endl;
    }

    // test pair: Green
    // test goal: POST with Header
    // test cmd : curl -X POST http://127.0.0.1:1984/api/v2/green
    auto request_green = std::make_shared<restbed::Request>(
                restbed::Uri("http://127.0.0.1:1984/api/v2/green"));
    request_green->set_header("Connection", "keep-alive");
    request_green->set_method("POST");
    request_green->set_header("Color", "Green");
    request_green->set_header("Index", "188");
    auto response_green = restbed::Http::sync(request_green);
    if (response_green->get_status_code() == 200) {
        auto clen = response_green->get_header("Content-Length", 0);
        auto bytes = restbed::Http::fetch(clen, response_green);
        std::string s(bytes.begin(), bytes.end());
        std::cout << s << std::endl;
    }

    // test pair: Blue
    // test goal: POST with application/json
    // test cmd : curl -X POST http://127.0.0.1:1984/api/v2/blue -H "Content-Type:application/json" -d'{"username" : "ultraboy", "userage" : "20394"}'
    auto request_blue = std::make_shared<restbed::Request>(
                restbed::Uri("http://127.0.0.1:1984/api/v2/blue"));
    std::string body = "{ \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] }";
    request_blue->set_header("Connection", "keep-alive");
    request_blue->set_method("POST");
    request_blue->set_body(body);
    request_blue->set_header("Content-Length", std::to_string(body.size()));
    request_blue->set_header("Content-Type", "application/json");
    auto response_blue = restbed::Http::sync(request_blue);
    if (response_blue->get_status_code() == 200) {
        auto clen = response_blue->get_header("Content-Length", 0);
        auto bytes = restbed::Http::fetch(clen, response_blue);
        std::string s(bytes.begin(), bytes.end());
        std::cout << s << std::endl;
    }  

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
