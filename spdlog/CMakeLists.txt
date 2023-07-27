
cmake_minimum_required(VERSION 3.5)

project("spdlog test" CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -g -O2 -std=c++11")

option(BUILD_DEFAULT            "build default"      OFF)
option(BUILD_LOGLEVEL           "build loglevel"     OFF)
option(BUILD_CUSTON             "build custom"       OFF)
option(BUILD_LOGFILE            "build log-file"     ON)

include_directories(../include)

if (BUILD_DEFAULT)
    add_executable(01_default  01_default.cpp)
endif()

if (BUILD_LOGLEVEL)
    add_executable(02_loglevel  02_loglevel.cpp)
endif()

if (BUILD_CUSTON)
    add_executable(03_custom  03_custom.cpp)
endif()

if (BUILD_LOGFILE)
    add_executable(04_logfile 04_logfile.cpp)
endif()
