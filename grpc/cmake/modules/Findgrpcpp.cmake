
find_library(grpc++_LIBRARY grpc++ "/usr/local/grpc/lib" "/usr/local/grpc/lib64" NO_DEFAULT_PATH)
find_library(grpc++_reflection_LIBRARY grpc++_reflection "/usr/local/grpc/lib" "/usr/local/grpc/lib64" NO_DEFAULT_PATH)

find_path(grpcpp_INCLUDE grpcpp/grpcpp.h HINTS "/usr/local/grpc/include" NO_DEFAULT_PATH)

if(grpcpp_INCLUDE AND grpc++_LIBRARY AND grpc++_reflection_LIBRARY)
    set(GRPCPP_FOUND TRUE)
    
    message(STATUS "${Green}Found grpcpp include at: ${grpcpp_INCLUDE}${Reset}")
    message(STATUS "${Green}Found grpcpp library at: ${grpc++_LIBRARY}${Reset}")
    message(STATUS "${Green}Found grpcpp library at: ${grpc++_reflection_LIBRARY}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate grpcpp dependency.${Reset}")
endif()
