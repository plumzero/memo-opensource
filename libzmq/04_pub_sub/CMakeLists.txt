
cmake_minimum_required(VERSION 2.8.0)

add_executable(pub_4 pub_4.cpp)
target_link_libraries(pub_4 ${zmq_LIBRARY_NAME})

add_executable(sub_4 sub_4.cpp)
target_link_libraries(sub_4 ${zmq_LIBRARY_NAME})
