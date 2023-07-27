
#include <cstdio>
#include <string>
#include <memory>

#include <grpcpp/grpcpp.h>

#include "echostream.grpc.pb.h"

// 参考文件
//  mock_test.cc
//  hybrid_end2end_test.cc
//  grpc_tool_test.cc

class EchoTestServiceImpl final : public echo::EchoTestService::Service
{
    // 双向流
    grpc::Status BidiStream(grpc::ServerContext* context, grpc::ServerReaderWriter< echo::EchoResponse, echo::EchoRequest>* stream) override
    {
        (void) context;
        echo::EchoRequest request;
        echo::EchoResponse response;
        while(stream->Read(&request)) {
            response.set_message("BidStream Hello " + request.message());
            stream->Write(response);
        }
        return grpc::Status::OK;
    }

    // 客户端流式传输
    grpc::Status RequestStream(grpc::ServerContext* context, grpc::ServerReader<echo::EchoRequest>* reader, echo::EchoResponse* response) override
    {
        (void) context;
        echo::EchoRequest request;

        while (reader->Read(&request)) {
            response->set_message("RequestStream Hello " + request.message());
        }

        return grpc::Status::OK;
    }
    
    // 服务端流式传输
    grpc::Status ResponseStream(grpc::ServerContext* context, const echo::EchoRequest* request, grpc::ServerWriter<echo::EchoResponse>* writer) override
    {
        (void) context;
        echo::EchoResponse response;

        response.set_message("ResponseStream Hello " + request->message());
        writer->Write(response);

        return grpc::Status::OK;
    }
};

int main(int argc, char * argv[])
{
    (void) argc;
    (void) argv;

    std::string server_address("0.0.0.0:50051");
    EchoTestServiceImpl service;
    
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
