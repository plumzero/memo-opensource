
#include <cstdio>
#include <string>
#include <memory>

#include <grpcpp/grpcpp.h>

#include "echostream.grpc.pb.h"

class EchoClient
{
public:
    // explicit EchoClient(echo::EchoTestService::StubInterface* stub)
    //     : stub_(stub) {}
    EchoClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(echo::EchoTestService::NewStub(channel)) {}

    void SendBidiStreaming()
    {
        echo::EchoRequest request;
        echo::EchoResponse reply;
        grpc::ClientContext context;
        context.set_wait_for_ready(true);
        request.set_message("Bid");

        std::unique_ptr<grpc::ClientReaderWriterInterface<echo::EchoRequest, echo::EchoResponse>> stream = stub_->BidiStream(&context);

        stream->Write(request);
        stream->Read(&reply);

        stream->WritesDone();

        grpc::Status status = stream->Finish();
        if (status.ok()) {
            printf("ok: %s\n", reply.message().c_str());
        } else {
            printf("grpc error: %s\n", status.error_message().c_str());
        }
    }

    void RequestStream()
    {
        echo::EchoRequest request;
        echo::EchoResponse reply;
        grpc::ClientContext context;
        context.set_wait_for_ready(true);
        request.set_message("Request");

        auto stream = stub_->RequestStream(&context, &reply);

        stream->Write(request);
        stream->WritesDone();

        grpc::Status status = stream->Finish();
        if (status.ok()) {
            printf("ok: %s\n", reply.message().c_str());
        } else {
            printf("grpc error: %s\n", status.error_message().c_str());
        }
    }

    void ResponseStream() {
        echo::EchoRequest request;
        echo::EchoResponse reply;
        grpc::ClientContext context;
        context.set_wait_for_ready(true);
        request.set_message("Response");

        auto stream = stub_->ResponseStream(&context, request);

        stream->Read(&reply);

        grpc::Status status = stream->Finish();
        if (status.ok()) {
            printf("ok: %s\n", reply.message().c_str());
        } else {
            printf("grpc error: %s\n", status.error_message().c_str());
        }
    }

private:
    std::unique_ptr<echo::EchoTestService::Stub> stub_;
};

int main(int argc, char * argv[])
{
    (void) argc;
    (void) argv;   

    EchoClient client(grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials()));

    client.SendBidiStreaming();

    client.RequestStream();

    client.ResponseStream();

    return 0;
}
