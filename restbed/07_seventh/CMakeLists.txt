
cmake_minimum_required(VERSION 2.8.10)

#
# json
#
add_executable(jsonclient jsonclient.cpp ${RestbedSRCS})
add_executable(jsonserver jsonserver.cpp ${RestbedSRCS})