cmake_minimum_required(VERSION 3.5.0)

add_executable(hello hello.cpp)
target_link_libraries(hello ${pmi_LIBRARY_NAME})
