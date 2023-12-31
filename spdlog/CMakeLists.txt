
cmake_minimum_required(VERSION 3.5)

project("spdlog test" CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -g -O2 -std=c++11")

option(BUILD_DEFAULT            "build default"         ON)
option(BUILD_LOGLEVEL           "build loglevel"        OFF)
option(BUILD_CUSTON             "build custom"          OFF)
option(BUILD_LOGFILE            "build log-file"        ON)
option(BUILD_DAILY              "build daily log"       ON)
option(BUILD_MONTHLY            "build monthly log"     ON)
option(BUILD_CLEARLOG           "build clear log"       ON)
option(BUILD_PERIODICAL         "build periodical log"  ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

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

if (BUILD_DAILY)
    add_executable(05_daily 05_daily.cpp)
endif()

if (BUILD_MONTHLY)
    add_executable(06_monthly 06_monthly.cpp)
endif()

if (BUILD_CLEARLOG)
    add_executable(07_clearlog 07_clearlog.cpp)
endif()

if (BUILD_PERIODICAL)
    add_executable(08_periodical 08_periodical.cpp)
endif()
