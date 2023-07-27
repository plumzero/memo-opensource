cmake_minimum_required (VERSION 3.0.2)

project ("pistache my test" CXX)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")

include_directories(../include)

aux_source_directory(../src/common COMMON_SOURCE_FILES)
aux_source_directory(../src/server SERVER_SOURCE_FILES)
aux_source_directory(../src/client CLIENT_SOURCE_FILES)

aux_source_directory(. SRC_LIST)

set_source_files_properties(${SRC_LIST} PROPERTIES LANGUAGE CXX)

foreach(src ${SRC_LIST})
    string(REGEX REPLACE "^./" "" src ${src})
    
    set(exe ${src})
    string(REGEX REPLACE ".cc$" "" exe ${exe})
    
    add_executable(${exe} ${src} ${COMMON_SOURCE_FILES} ${SERVER_SOURCE_FILES} ${CLIENT_SOURCE_FILES})
    target_link_libraries(${exe} pthread)
endforeach(src)