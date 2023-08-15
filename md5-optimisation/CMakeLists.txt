
cmake_minimum_required(VERSION 3.5)

# -ggdb -Wall -Wextra -fno-rtti -fno-exceptions -std=c++11 -O3 -static

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -Wall -Wextra -fno-rtti -fno-exceptions -std=c++11 -O3 -static")

include_directories(.)

add_executable(md5 md5.cpp)

add_executable(md5sum md5sum.cpp)
