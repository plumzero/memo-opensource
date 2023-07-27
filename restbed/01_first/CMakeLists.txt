
cmake_minimum_required(VERSION 2.8.10)

#
# GET
#
add_executable(getclient getclient.cpp ${RestbedSRCS})
add_executable(getserver getserver.cpp ${RestbedSRCS})