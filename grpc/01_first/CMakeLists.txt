cmake_minimum_required(VERSION 3.5.0)


## 依次执行如下两条命令

## 消息文件
## /usr/local/grpc/bin/protoc --proto_path=. --cpp_out=. helloworld.proto

## 服务文件
## LD_LIBRARY_PATH=/usr/local/grpc/lib64:/usr/local/grpc/lib:$LD_LIBRARY_PATH /usr/local/grpc/bin/protoc 
##                  --plugin=protoc-gen-grpc=/usr/local/grpc/bin/grpc_cpp_plugin --proto_path=. --grpc_out=. helloworld.proto
 

include_directories(.)

add_executable(greeter_client greeter_client.cpp helloworld.pb.cc helloworld.grpc.pb.cc)
target_link_libraries(greeter_client ${protobuf_LIBRARY_NAME} ${grpc_LIBRARY_NAME} ${grpc++_LIBRARY_NAME} ${grpc++_reflection})

add_executable(greeter_server greeter_server.cpp helloworld.pb.cc helloworld.grpc.pb.cc)
target_link_libraries(greeter_server ${protobuf_LIBRARY_NAME} ${grpc_LIBRARY_NAME} ${grpc++_LIBRARY_NAME} ${grpc++_reflection})
