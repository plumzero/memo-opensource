
find_library(protobuf_LIBRARY protobuf HINTS "/usr/local/protobuf/lib")

find_path(protobuf_INCLUDE google/protobuf/message.h HINTS "/usr/local/protobuf/include")

if(protobuf_LIBRARY AND protobuf_INCLUDE)
    set(PROTOBUF_FOUND TRUE)
    
    message(STATUS "${Green}Found protobuf library at: ${protobuf_LIBRARY}${Reset}")
    message(STATUS "${Green}Found protobuf include at: ${protobuf_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate protobuf dependency.${Reset}")
endif()
