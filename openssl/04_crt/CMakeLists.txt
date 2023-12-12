cmake_minimum_required(VERSION 3.5.0)

file(COPY cert.ca.pem DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY cert.sub.pem DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(crt_parse crt_parse.cpp)
target_link_libraries(crt_parse ${crypto_LIBRARY_NAME})

add_executable(crt_verify crt_verify.cpp)
target_link_libraries(crt_verify ${crypto_LIBRARY_NAME})

add_executable(crt_chain_verify crt_chain_verify.cpp)
target_link_libraries(crt_chain_verify ${crypto_LIBRARY_NAME})

add_executable(crt_pri_match crt_pri_match.cpp)
target_link_libraries(crt_pri_match ${crypto_LIBRARY_NAME})
