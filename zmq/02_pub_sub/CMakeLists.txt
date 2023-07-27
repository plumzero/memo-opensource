
cmake_minimum_required(VERSION 2.8.0)

add_executable(pub_2 pub_2.cpp)
target_link_libraries(pub_2 ${zmq_LIBRARY_NAME})

add_executable(sub_2 sub_2.cpp)
target_link_libraries(sub_2 ${zmq_LIBRARY_NAME})
