
find_path(zmqhpp_INCLUDE zmq.hpp HINTS ${PROJECT_SOURCE_DIR} NO_DEFAULT_PATH)

if(zmqhpp_INCLUDE)
    set(ZMQHPP_FOUND TRUE)
    
    message(STATUS "${Green}Found zmqhpp include at: ${zmqhpp_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate zmqhpp dependency.${Reset}")
endif()
