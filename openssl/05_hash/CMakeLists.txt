cmake_minimum_required(VERSION 3.5.0)

add_executable(sha256_hmac sha256_hmac.cpp)
target_link_libraries(sha256_hmac ${crypto_LIBRARY_NAME})
