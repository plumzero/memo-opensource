
#include <cstdio>
#include <string>
#include <memory>
#include <map>

#include <grpcpp/grpcpp.h>

#include "helloworld.grpc.pb.h"

class GreeterServiceImpl final : public helloworld::Greeter::Service
{
    grpc::Status SayHello(grpc::ServerContext * context,
                          const helloworld::HelloRequest * request,
                          helloworld::HelloReply * reply) override
    {   
        const std::multimap<grpc::string_ref, grpc::string_ref> metadata = context->client_metadata();
        
        printf("metadata.size=%lu\n", metadata.size());
        
        decltype(metadata.begin()) iter;
        
        iter = metadata.find("custom-header");
        if (iter != metadata.end()) {
            printf("%s : %s\n", iter->first.data(), std::string(iter->second.data(), iter->second.size()).c_str());
        }
        
        iter = metadata.find("custom-bin");
        if (iter != metadata.end()) {
            printf("%s : ", iter->first.data());
            decltype(iter->second.size()) i;
            for (i = 0; i < iter->second.size(); i++) {
                printf("%02x ", iter->second.data()[i]);
            }
            printf("\n");
        }
        
        std::string prefix("Hello ");
        reply->set_message(prefix + request->name());
        
        return grpc::Status::OK;
    }
};

int main(int argc, char * argv[])
{
    (void) argc;
    (void) argv;

    std::string server_address("0.0.0.0:50051");
    GreeterServiceImpl service;
    
    grpc::ServerBuilder builder;
    
    /** 设置监听端口 */
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    /** 注册服务处理句柄 - 同步处理 */
    builder.RegisterService(&service);
    /** 在绑定的端口上启动服务(一个端口上只能启动一个服务，用 std::unique_ptr) */
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    printf("Server listening on %s\n", server_address.c_str());
    
    server->Wait();
    
    return 0;
}
