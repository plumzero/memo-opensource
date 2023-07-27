cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. trade.proto

include_directories(.)

add_executable(testTradeOrder testTradeOrder.cpp trade.pb.cc)
target_link_libraries(testTradeOrder ${protobuf_LIBRARY_NAME})

add_executable(testCancelOrder testCancelOrder.cpp trade.pb.cc)
target_link_libraries(testCancelOrder ${protobuf_LIBRARY_NAME})

add_executable(testTradeOrderRsp testTradeOrderRsp.cpp trade.pb.cc)
target_link_libraries(testTradeOrderRsp ${protobuf_LIBRARY_NAME})

add_executable(testTradeAccountQry testTradeAccountQry.cpp trade.pb.cc)
target_link_libraries(testTradeAccountQry ${protobuf_LIBRARY_NAME})

add_executable(testTradeAccountRsp testTradeAccountRsp.cpp trade.pb.cc)
target_link_libraries(testTradeAccountRsp ${protobuf_LIBRARY_NAME})
