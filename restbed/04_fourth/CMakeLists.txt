
cmake_minimum_required(VERSION 2.8.10)

#
# multiple request
#
add_executable(multiclient multiclient.cpp ${RestbedSRCS})
add_executable(multiserver multiserver.cpp ${RestbedSRCS})