cmake_minimum_required(VERSION 2.8.0)

add_executable(req req.cpp)
target_link_libraries(req ${zmq_LIBRARY_NAME})

add_executable(rep rep.cpp)
target_link_libraries(rep ${zmq_LIBRARY_NAME})

