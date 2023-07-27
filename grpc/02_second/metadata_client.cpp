
#include <cstdio>
#include <string>

#include <grpcpp/grpcpp.h>

#include "helloworld.grpc.pb.h"

class GreeterClient
{
public:
    GreeterClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(helloworld::Greeter::NewStub(channel)) {}
        
    std::string RequestSayHello(const std::string& user)
    {
        helloworld::HelloRequest request;

        request.set_name(user);
        
        helloworld::HelloReply reply;
        grpc::ClientContext context;
        
		context.AddMetadata("custom-header", "Custom Value");
		context.AddMetadata("custom-bin", std::string{'\x00', '\x01', '\x02', '\x25', '\x00'});
		
        /** 客户端调用与服务端某个服务同名的函数，向服务端的这个服务发送请求 */
        grpc::Status status = stub_->SayHello(&context, request, &reply);
        if (status.ok()) {
            return reply.message();
        } else {
            printf("error %d: %s\n", status.error_code(), status.error_message().c_str());
            return "RPC failed";
        }
    }
    
private:
    std::unique_ptr<helloworld::Greeter::Stub> stub_;
};

int main(int argc, char * argv[])
{
    (void) argc;
    (void) argv;   

    GreeterClient greeter(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));

    std::string user("world");
    std::string reply = greeter.RequestSayHello(user);
    
    printf("Greeter Client received: %s\n", reply.c_str());
    
    return 0;
}
