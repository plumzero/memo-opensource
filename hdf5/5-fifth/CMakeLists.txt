
cmake_minimum_required(VERSION 3.12)

add_executable(intarrayinstruct intarrayinstruct.cpp)
target_link_libraries(intarrayinstruct ${hdf5_LIBRARY_NAME} ${hdf5cpp_LIBRARY_NAME})

