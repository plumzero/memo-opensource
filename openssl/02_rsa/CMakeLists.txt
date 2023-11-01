cmake_minimum_required(VERSION 3.5.0)

add_executable(rsa_key_save rsa_key_save.cpp)
target_link_libraries(rsa_key_save ${crypto_LIBRARY_NAME})

add_executable(rsa_key_load rsa_key_load.cpp)
target_link_libraries(rsa_key_load ${crypto_LIBRARY_NAME})

add_executable(rsa_enc_dec rsa_enc_dec.cpp)
target_link_libraries(rsa_enc_dec ${crypto_LIBRARY_NAME})

add_executable(rsa_enc_dec2 rsa_enc_dec2.cpp)
target_link_libraries(rsa_enc_dec2 ${crypto_LIBRARY_NAME})

add_executable(rsa_sign_verify rsa_sign_verify.cpp)
target_link_libraries(rsa_sign_verify ${crypto_LIBRARY_NAME})
