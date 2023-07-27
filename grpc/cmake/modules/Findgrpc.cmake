
find_library(grpc_LIBRARY grpc HINTS "/usr/local/grpc/lib" "/usr/local/grpc/lib64" NO_DEFAULT_PATH)

find_path(grpc_INCLUDE grpc/grpc.h HINTS "/usr/local/grpc/include" NO_DEFAULT_PATH)

if(grpc_LIBRARY AND grpc_INCLUDE)
    set(GRPC_FOUND TRUE)
    
    message(STATUS "${Green}Found grpc library at: ${grpc_LIBRARY}${Reset}")
    message(STATUS "${Green}Found grpc include at: ${grpc_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate grpc dependency.${Reset}")
endif()

