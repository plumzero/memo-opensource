
cmake_minimum_required(VERSION 3.12)

add_executable(1_intarray 1_intarray.cpp)
target_link_libraries(1_intarray ${hdf5_LIBRARY_NAME} ${hdf5cpp_LIBRARY_NAME})

add_executable(2_intarray 2_intarray.cpp)
target_link_libraries(2_intarray ${hdf5_LIBRARY_NAME} ${hdf5cpp_LIBRARY_NAME})

add_executable(3_intarray 3_intarray.cpp)
target_link_libraries(3_intarray ${hdf5_LIBRARY_NAME} ${hdf5cpp_LIBRARY_NAME})