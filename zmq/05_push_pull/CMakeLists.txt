cmake_minimum_required(VERSION 2.8.0)

add_executable(dispatcher dispatcher.cpp)
target_link_libraries(dispatcher ${zmq_LIBRARY_NAME})

add_executable(processor processor.cpp)
target_link_libraries(processor ${zmq_LIBRARY_NAME})

add_executable(collector collector.cpp)
target_link_libraries(collector ${zmq_LIBRARY_NAME})
