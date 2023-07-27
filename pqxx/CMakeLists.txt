
cmake_minimum_required(VERSION 3.0.2)

project("pqxx" CXX)

include(CheckCXXCompilerFlag)

#
# Color Configuration
#
string(ASCII 27 Esc)
set(Reset   "${Esc}[m"  )
set(Red     "${Esc}[31m")
set(Blue    "${Esc}[34m")
set(Green   "${Esc}[32m")
set(Yellow  "${Esc}[33m")

#
# Compile Mode
#
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -Wall -W")
else()
    string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -W")
endif()

#
# PostgreSql
#
find_library(pgsql_LIBRARY pq HINTS "/usr/local/pgsql/lib" NO_DEFAULT_PATH)
find_path(pgsql_INCLUDE pgtypes.h HINTS "/usr/local/pgsql/include" NO_DEFAULT_PATH)

if (pgsql_LIBRARY AND pgsql_INCLUDE)
    set(PGSQL_FOUND TRUE)
    message(STATUS "${Green}Found pgsql library at: ${pgsql_LIBRARY}${Reset}")
    message(STATUS "${Green}Found pgsql include at: ${pgsql_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate pgsql.${Reset}" )
endif()

#
# libpqxx
#
find_library(pqxx_LIBRARY pqxx HINTS "/usr/local/pqxx/lib" NO_DEFAULT_PATH)
find_path(pqxx_INCLUDE pqxx/pqxx HINTS "/usr/local/pqxx/include" NO_DEFAULT_PATH)

if (pqxx_LIBRARY AND pqxx_INCLUDE)
    set(PQXX_FOUND TRUE)
    message(STATUS "${Green}Found pqxx library at: ${pqxx_LIBRARY}${Reset}")
    message(STATUS "${Green}Found pqxx include at: ${pqxx_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate pqxx.${Reset}" )
endif()

#
# Library PATH and NAME
#
get_filename_component(pgsql_LIBRARY_PATH ${pgsql_LIBRARY} PATH)
get_filename_component(pgsql_LIBRARY_NAME ${pgsql_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" pgsql_LIBRARY_NAME ${pgsql_LIBRARY_NAME})

get_filename_component(pqxx_LIBRARY_PATH ${pqxx_LIBRARY} PATH)
get_filename_component(pqxx_LIBRARY_NAME ${pqxx_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" pqxx_LIBRARY_NAME ${pqxx_LIBRARY_NAME})

#
# include and link
#
include_directories(SYSTEM ${pgsql_INCLUDE})
include_directories(SYSTEM ${pqxx_INCLUDE})

link_directories(${pqxx_LIBRARY_PATH})
link_directories(${pgsql_LIBRARY_PATH})

add_subdirectory(tests)
