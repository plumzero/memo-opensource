
cmake_minimum_required(VERSION 3.12)

add_executable(structarray structarray.cpp)
target_link_libraries(structarray ${hdf5_LIBRARY_NAME} ${hdf5cpp_LIBRARY_NAME})

