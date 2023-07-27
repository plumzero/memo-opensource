
cmake_minimum_required(VERSION 2.8.0)

add_executable(pub_3 pub_3.cpp)
target_link_libraries(pub_3 ${zmq_LIBRARY_NAME})

add_executable(sub_3 sub_3.cpp)
target_link_libraries(sub_3 ${zmq_LIBRARY_NAME})
