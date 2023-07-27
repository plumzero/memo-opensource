
cmake_minimum_required(VERSION 3.12)

add_executable(1_matrix 1_matrix.cpp)
target_link_libraries(1_matrix ${hdf5_LIBRARY_NAME} ${hdf5cpp_LIBRARY_NAME})

add_executable(2_matrix 2_matrix.cpp)
target_link_libraries(2_matrix ${hdf5_LIBRARY_NAME} ${hdf5cpp_LIBRARY_NAME})