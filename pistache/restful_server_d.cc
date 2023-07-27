/* 
   Mathieu Stefani, 07 février 2016
   
   Example of a REST endpoint with routing
*/

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <algorithm>
#include <iomanip>

/**
 * 在服务类 ServiceEndPoint 中添加一些统计量
 *  resolver_counter
 *  notfound_counter
 *  reject_counter
 */

static const char tchs[] = {
    "LyogaHR0cC5oCiAgIE1hdGhpZXUgU3RlZmFuaSwgMTMgQXVndXN0IDIwMTUKICAgCiAgIEh0dHAgTGF5ZXIKKi8KCiNwcmFnb"
    "WEgb25jZQoKI2luY2x1ZGUgPHN5cy90aW1lcmZkLmg+CiNpbmNsdWRlIDx0eXBlX3RyYWl0cz4KI2luY2x1ZGUgPHN0ZGV4Y2"
    "VwdD4KI2luY2x1ZGUgPGFycmF5PgojaW5jbHVkZSA8c3N0cmVhbT4KI2luY2x1ZGUgIm5ldC5oIgojaW5jbHVkZSAiaHR0cF9"
    "oZWFkZXJzLmgiCiNpbmNsdWRlICJodHRwX2RlZnMuaCIKI2luY2x1ZGUgImNvb2tpZS5oIgojaW5jbHVkZSAic3RyZWFtLmgi"
    "CiNpbmNsdWRlICJtaW1lLmgiCiNpbmNsdWRlICJhc3luYy5oIgojaW5jbHVkZSAicGVlci5oIgojaW5jbHVkZSAidGNwLmgiC"
    "iNpbmNsdWRlICJ0cmFuc3BvcnQuaCIKCm5hbWVzcGFjZSBOZXQgewoKbmFtZXNwYWNlIEh0dHAgewoKbmFtZXNwYWNlIFByaX"
    "ZhdGUgewogICAgY2xhc3MgUGFyc2VyQmFzZTsKICAgIHRlbXBsYXRlPHR5cGVuYW1lIFQ+IHN0cnVjdCBQYXJzZXI7CiAgICB"
    "jbGFzcyBSZXF1ZXN0TGluZVN0ZXA7CiAgICBjbGFzcyBSZXNwb25zZUxpbmVTdGVwOwogICAgY2xhc3MgSGVhZGVyc1N0ZXA7"
    "CiAgICBjbGFzcyBCb2R5U3RlcDsKfQoKbmFtZXNwYWNlIEV4cGVyaW1lbnRhbCB7CiAgICBjbGFzcyBDbGllbnQ7CiAgICBjb"
    "GFzcyBSZXF1ZXN0QnVpbGRlcjsKfQoKdGVtcGxhdGU8IGNsYXNzIENoYXJULCBjbGFzcyBUcmFpdHM+CnN0ZDo6YmFzaWNfb3"
    "N0cmVhbTxDaGFyVCwgVHJhaXRzPiYgY3JsZihzdGQ6OmJhc2ljX29zdHJlYW08Q2hhclQsIFRyYWl0cz4mIG9zKSB7CiAgICB"
    "zdGF0aWMgY29uc3RleHByIGNoYXIgQ1JMRltdID0gezB4RCwgMHhBfTsKICAgIG9zLndyaXRlKENSTEYsIDIpOwoKICAgIHJl"
    "dHVybiBvczsKfQoKLy8gNC4gSFRUUCBNZXNzYWdlCmNsYXNzIE1lc3NhZ2UgewpwdWJsaWM6CiAgICBmcmllbmQgY2xhc3MgU"
    "HJpdmF0ZTo6SGVhZGVyc1N0ZXA7CiAgICBmcmllbmQgY2xhc3MgUHJpdmF0ZTo6Qm9keVN0ZXA7CiAgICBmcmllbmQgY2xhc3"
    "MgUHJpdmF0ZTo6UGFyc2VyQmFzZTsKCiAgICBNZXNzYWdlKCk7CgogICAgTWVzc2FnZShjb25zdCBNZXNzYWdlJiBvdGhlcik"
    "gPSBkZWZhdWx0OwogICAgTWVzc2FnZSYgb3BlcmF0b3I9KGNvbnN0IE1lc3NhZ2UmIG90aGVyKSA9IGRlZmF1bHQ7CgogICAg"
    "TWVzc2FnZShNZXNzYWdlJiYgb3RoZXIpID0gZGVmYXVsdDsKICAgIE1lc3NhZ2UmIG9wZXJhdG9yPShNZXNzYWdlJiYgb3RoZ"
    "XIpID0gZGVmYXVsdDsKCnByb3RlY3RlZDoKICAgIFZlcnNpb24gdmVyc2lvbl87CiAgICBDb2RlIGNvZGVfOwoKICAgIEhlYW"
    "Rlcjo6Q29sbGVjdGlvbiBoZWFkZXJzXzsKICAgIHN0ZDo6c3RyaW5nIGJvZHlfOwoKICAgIENvb2tpZUphciBjb29raWVzXzs"
    "KfTsKCm5hbWVzcGFjZSBVcmkgewogICAgdHlwZWRlZiBzdGQ6OnN0cmluZyBGcmFnbWVudDsKCiAgICBjbGFzcyBRdWVyeSB7"
    "KfTsKCm5hbWVzcGFjZSBVcmkgewogICAgdHlwZWRlZ"
    "LyogaHR0cC5oCiAgIE1hdGhpZXUgU3RlZmFuaSwgMTMgQXVndXN0IDIwMTUKICAgCiAgIEh0dHAgTGF5ZXIKKi8KCiNwcmFnb"
    "WEgb25jZQoKI2luY2x1ZGUgPHN5cy90aW1lcmZkLmg+CiNpbmNsdWRlIDx0eXBlX3RyYWl0cz4KI2luY2x1ZGUgPHN0ZGV4Y2"
    "VwdD4KI2luY2x1ZGUgPGFycmF5PgojaW5jbHVkZSA8c3N0cmVhbT4KI2luY2x1ZGUgIm5ldC5oIgojaW5jbHVkZSAiaHR0cF9"
    "oZWFkZXJzLmgiCiNpbmNsdWRlICJodHRwX2RlZnMuaCIKI2luY2x1ZGUgImNvb2tpZS5oIgojaW5jbHVkZSAic3RyZWFtLmgi"
    "CiNpbmNsdWRlICJtaW1lLmgiCiNpbmNsdWRlICJhc3luYy5oIgojaW5jbHVkZSAicGVlci5oIgojaW5jbHVkZSAidGNwLmgiC"
    "iNpbmNsdWRlICJ0cmFuc3BvcnQuaCIKCm5hbWVzcGFjZSBOZXQgewoKbmFtZXNwYWNlIEh0dHAgewoKbmFtZXNwYWNlIFByaX"
    "ZhdGUgewogICAgY2xhc3MgUGFyc2VyQmFzZTsKICAgIHRlbXBsYXRlPHR5cGVuYW1lIFQ+IHN0cnVjdCBQYXJzZXI7CiAgICB"
    "jbGFzcyBSZXF1ZXN0TGluZVN0ZXA7CiAgICBjbGFzcyBSZXNwb25zZUxpbmVTdGVwOwogICAgY2xhc3MgSGVhZGVyc1N0ZXA7"
    "CiAgICBjbGFzcyBCb2R5U3RlcDsKfQoKbmFtZXNwYWNlIEV4cGVyaW1lbnRhbCB7CiAgICBjbGFzcyBDbGllbnQ7CiAgICBjb"
    "GFzcyBSZXF1ZXN0QnVpbGRlcjsKfQoKdGVtcGxhdGU8IGNsYXNzIENoYXJULCBjbGFzcyBUcmFpdHM+CnN0ZDo6YmFzaWNfb3"
    "N0cmVhbTxDaGFyVCwgVHJhaXRzPiYgY3JsZihzdGQ6OmJhc2ljX29zdHJlYW08Q2hhclQsIFRyYWl0cz4mIG9zKSB7CiAgICB"
    "zdGF0aWMgY29uc3RleHByIGNoYXIgQ1JMRltdID0gezB4RCwgMHhBfTsKICAgIG9zLndyaXRlKENSTEYsIDIpOwoKICAgIHJl"
    "dHVybiBvczsKfQoKLy8gNC4gSFRUUCBNZXNzYWdlCmNsYXNzIE1lc3NhZ2UgewpwdWJsaWM6CiAgICBmcmllbmQgY2xhc3MgU"
    "HJpdmF0ZTo6SGVhZGVyc1N0ZXA7CiAgICBmcmllbmQgY2xhc3MgUHJpdmF0ZTo6Qm9keVN0ZXA7CiAgICBmcmllbmQgY2xhc3"
    "MgUHJpdmF0ZTo6UGFyc2VyQmFzZTsKCiAgICBNZXNzYWdlKCk7CgogICAgTWVzc2FnZShjb25zdCBNZXNzYWdlJiBvdGhlcik"
    "gPSBkZWZhdWx0OwogICAgTWVzc2FnZSYgb3BlcmF0b3I9KGNvbnN0IE1lc3NhZ2UmIG90aGVyKSA9IGRlZmF1bHQ7CgogICAg"
    "TWVzc2FnZShNZXNzYWdlJiYgb3RoZXIpID0gZGVmYXVsdDsKICAgIE1lc3NhZ2UmIG9wZXJhdG9yPShNZXNzYWdlJiYgb3RoZ"
    "XIpID0gZGVmYXVsdDsKCnByb3RlY3RlZDoKICAgIFZlcnNpb24gdmVyc2lvbl87CiAgICBDb2RlIGNvZGVfOwoKICAgIEhlYW"
    "Rlcjo6Q29sbGVjdGlvbiBoZWFkZXJzXzsKICAgIHN0ZDo6c3RyaW5nIGJvZHlfOwoKICAgIENvb2tpZUphciBjb29raWVzXzs"
    "KfTsKCm5hbWVzcGFjZSBVcmkgewogICAgdHlwZWRlZiBzdGQ6OnN0cmluZyBGcmFnbWVudDsKCiAgICBjbGFzcyBRdWVyeSB7"
    "KfTsKCm5hbWVzcGFjZSBVcmkgewogICAgdHlwZWRlZ"
};

class ServiceEndPoint
{
public:
    ServiceEndPoint(Pistache::Address addr)
        : httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr)) 
        , counter_ok(0)
        , counter_notfound(0) { }

    void init(size_t thr = 2)
    {
        auto opts = Pistache::Http::Endpoint::options()
            .flags(Pistache::Tcp::Options::ReuseAddr)
            .threads(thr);
            // .flags(Pistache::Tcp::Options::InstallSignalHandler);
        httpEndpoint->init(opts);
        setupRoutes();      
    }
    
    void start()
    {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serveThreaded();
    }

    void shutdown()
    {
        httpEndpoint->shutdown();
    }

    void statistics()
    {
        
        std::cout << "statistics: ok=" << counter_ok << ", notfound=" << counter_notfound << std::endl;
    }
    
private:
    /** 设置本服务的资源点(url + http 方法 + 回调句柄) */
    void setupRoutes()
    {
        Pistache::Rest::Routes::NotFound(router, 
          [this](const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
            std::string err{"Couldn't find route: \"" + request.resource() + "\"\n"};
            response.send(Pistache::Http::Code::Not_Found, err);
            ++counter_notfound;
            return Pistache::Rest::Route::Result::Ok;
        });
        Pistache::Rest::Routes::Get(router, "/api/v1/micro/service/Hello",
                    Pistache::Rest::Routes::bind(&ServiceEndPoint::doServiceHello, this));
        
        Pistache::Rest::Routes::Get(router, "/test", 
                    Pistache::Rest::Routes::bind(&ServiceEndPoint::doServiceTest, this));
    }
    
    void doServiceTest(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
        response.send(Pistache::Http::Code::Ok, "Hello World!");
    }
    
    void doServiceHello(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
        response.send(Pistache::Http::Code::Ok, std::string(tchs));
        std::cout << std::setw(4) << ++counter_ok 
                  << "  recv \"" << request.body()
                  << "\",  send " << sizeof(tchs) << " bytes" << std::endl;
    }

    std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint;
    Pistache::Rest::Router router;
    int counter_ok;
    int counter_notfound;
};

int main(int argc, char *argv[])
{
    int thr = 2;
    std::cout << "Using " << thr << " threads" << std::endl;

    std::cout.fill('0');
    
    ServiceEndPoint service1(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(8090)));
    service1.init(thr);
    service1.start();
    
    ServiceEndPoint service2(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(8091)));
    service2.init(thr);
    service2.start();
    
    sleep(30);
    
    service1.statistics();
    service2.statistics();
    
    service1.shutdown();
    service2.shutdown();
    
    return 0;
}
