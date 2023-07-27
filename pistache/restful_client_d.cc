
/* 
   Mathieu Stefani, 07 février 2016
   
 * Http client example
*/

#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/client.h>
#include <pistache/http_headers.h>

#include <iomanip>

int main(int argc, char *argv[])
{
    Pistache::Http::Client client;

    auto opts = Pistache::Http::Client::options()
        .threads(2)
        .maxConnectionsPerHost(32)
        .keepAlive(true);
    client.init(opts);
    
    int i = 0;
    
    std::cout.fill('0');
    
    struct timeval btv, etv;
    double bmillis, emillis;
    
    gettimeofday(&btv, 0);
    bmillis = (double)btv.tv_sec * 1000 + (double)btv.tv_usec / 1000;
        
    std::string queryStr;
    
    int total = 6000;
    int counter_ok = 0, counter_err = 0;
    
    for (i = 0; i < total; i++) {
        std::string page;
        if (i % 2) {
            page = "http://192.168.43.228:8090/api/v1/micro/service/Hello";
        } else {
            page = "http://192.168.43.228:8091/api/v1/micro/service/Hello";
        }

        auto resp = client
                .get(page)
                .header<Pistache::Http::Header::Connection>(Pistache::Http::ConnectionControl::KeepAlive)
                .body("abcd")
                .send();
        
        resp.then([=, &counter_ok, &counter_err, &queryStr](Pistache::Http::Response response) {
            if (response.code() == Pistache::Http::Code::Ok) {
                // queryStr = response.body();
                // std::cout << std::setw(4) << i + 1 << " body size = " << queryStr.size() << std::endl;
                // auto body = response.body();
                // if (!body.empty())
                    // std::cout << "Response body = " << body << std::endl;
                ++counter_ok;
            } else {
                std::cout << std::setw(4) << i + 1 << " error occured" << std::endl;
                ++counter_err;
            }
            
        }, Pistache::Async::IgnoreException);   // NoExcept  IgnoreException   Throw
        
        Pistache::Async::Barrier<Pistache::Http::Response> barrier(resp);
        barrier.wait_for(std::chrono::milliseconds(50));
    }
    
    gettimeofday(&etv, 0);
    emillis = (double)etv.tv_sec * 1000 + (double)etv.tv_usec / 1000;
    
    double consumed = (emillis - bmillis) / 1000;
    
    printf("%d requests consumed time = %.3lf seconds\n", total, consumed);
    printf("statistics: ok=%d, err=%d, tps = %.3lf\n", counter_ok, counter_err, (double)total / consumed);
    
    client.shutdown();
    
    return 0;
}
  