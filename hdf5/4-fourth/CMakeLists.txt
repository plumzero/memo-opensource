
cmake_minimum_required(VERSION 3.12)

add_executable(charsinstruct charsinstruct.cpp)
target_link_libraries(charsinstruct ${hdf5_LIBRARY_NAME} ${hdf5cpp_LIBRARY_NAME})

