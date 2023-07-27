
cmake_minimum_required(VERSION 3.12)

add_executable(oop oop.cpp)
target_link_libraries(oop ${hdf5_LIBRARY_NAME} ${hdf5cpp_LIBRARY_NAME})

