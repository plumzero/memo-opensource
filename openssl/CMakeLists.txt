cmake_minimum_required(VERSION 3.5.0)

project("openssl test" C CXX)

option(BUILD_1_AES              "Build aes test"          ON)
option(BUILD_2_RSA              "Build rsa test"          ON)
option(BUILD_3_CSR              "Build csr test"          ON)
option(BUILD_4_CRT              "Build crt test"          ON)
#
# if u wanna release, do like this `cmake -DCMAKE_BUILD_TYPE=Release`
#
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(CheckCXXCompilerFlag)

#
# Configuration
#
include("${PROJECT_SOURCE_DIR}/cmake/configuration.cmake")

#
# Dependencies
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

#
# Package - openssl
#
find_package(crypto REQUIRED)
include_directories(SYSTEM ${crypto_INCLUDE})
get_filename_component(crypto_LIBRARY_PATH ${crypto_LIBRARY} PATH)
get_filename_component(crypto_LIBRARY_NAME ${crypto_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" crypto_LIBRARY_NAME ${crypto_LIBRARY_NAME})

link_directories(${crypto_LIBRARY_PATH})

#
# Test in Sub Directories
#
if(BUILD_1_AES)
    add_subdirectory(01_aes)
endif()

if(BUILD_2_RSA)
    add_subdirectory(02_rsa)
endif()

if(BUILD_3_CSR)
    add_subdirectory(03_csr)
endif()

if(BUILD_4_CRT)
    add_subdirectory(04_crt)
endif()

#
# End
#
