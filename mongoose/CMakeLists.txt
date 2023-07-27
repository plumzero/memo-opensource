
cmake_minimum_required(VERSION 2.6)

project("mongoose test" C)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -W ")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -w")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMG_ENABLE_CALLBACK_USERDATA=1")

set_source_files_properties(${SRC_LIST} PROPERTIES LANGUAGE C)

include_directories(..)

add_executable(restful_client restful_client.c ../mongoose.c)
add_executable(restful_server restful_server.c ../mongoose.c)
