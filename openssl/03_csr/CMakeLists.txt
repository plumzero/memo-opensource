cmake_minimum_required(VERSION 3.5.0)

add_executable(csr_create csr_create.cpp)
target_link_libraries(csr_create ${crypto_LIBRARY_NAME})

add_executable(csr_parse csr_parse.cpp)
target_link_libraries(csr_parse ${crypto_LIBRARY_NAME})
