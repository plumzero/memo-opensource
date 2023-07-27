
cmake_minimum_required(VERSION 2.8.10)

#
# thread server
#
add_executable(threadclient threadclient.cpp ${RestbedSRCS})
add_executable(threadserver threadserver.cpp ${RestbedSRCS})
