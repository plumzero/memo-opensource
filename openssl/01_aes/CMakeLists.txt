cmake_minimum_required(VERSION 3.5.0)

add_executable(aes_128_ecb_nopadding aes_128_ecb_nopadding.cpp)
target_link_libraries(aes_128_ecb_nopadding ${crypto_LIBRARY_NAME})

add_executable(aes_128_cbc_nopadding aes_128_cbc_nopadding.cpp)
target_link_libraries(aes_128_cbc_nopadding ${crypto_LIBRARY_NAME})

add_executable(level2_calc level2_calc.cpp)
target_link_libraries(level2_calc ${crypto_LIBRARY_NAME})