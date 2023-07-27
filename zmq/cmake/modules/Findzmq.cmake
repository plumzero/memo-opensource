
find_library(zmq_LIBRARY zmq HINTS "/usr/local/zmq/lib" NO_DEFAULT_PATH)

find_path(zmq_INCLUDE zmq.h zmq_utils.h HINTS "/usr/local/zmq/include" NO_DEFAULT_PATH)

if(zmq_LIBRARY AND zmq_INCLUDE)
    set(ZMQ_FOUND TRUE)
    
    message(STATUS "${Green}Found zmq library at: ${zmq_LIBRARY}${Reset}")
    message(STATUS "${Green}Found zmq include at: ${zmq_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate zmq dependency.${Reset}")
endif()
