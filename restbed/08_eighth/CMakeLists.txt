
cmake_minimum_required(VERSION 2.8.10)

#
# test request
#
add_executable(testclient testclient.cpp ${RestbedSRCS})
add_executable(testserver testserver.cpp ${RestbedSRCS})