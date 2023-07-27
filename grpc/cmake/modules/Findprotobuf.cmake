
find_library(protobuf_LIBRARY protobuf HINTS "/usr/local/grpc/lib" "/usr/local/grpc/lib64" "/usr/local/protobuf/lib" NO_DEFAULT_PATH)

find_path(protobuf_INCLUDE google/protobuf/message.h HINTS "/usr/local/grpc/include" "/usr/local/protobuf/include" NO_DEFAULT_PATH)

if(protobuf_LIBRARY AND protobuf_INCLUDE)
    set(PROTOBUF_FOUND TRUE)
    
    message(STATUS "${Green}Found protobuf library at: ${protobuf_LIBRARY}${Reset}")
    message(STATUS "${Green}Found protobuf include at: ${protobuf_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate protobuf dependency.${Reset}")
endif()
