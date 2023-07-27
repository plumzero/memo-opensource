
cmake_minimum_required(VERSION 2.8.0)

add_executable(04_smtp  04_smtp.cpp)
target_link_libraries(04_smtp ${curl_LIBRARY_NAME})
