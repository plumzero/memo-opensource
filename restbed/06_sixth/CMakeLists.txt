
cmake_minimum_required(VERSION 2.8.10)

#
# client send body message
#
add_executable(bodyclient bodyclient.cpp ${RestbedSRCS})
add_executable(bodyserver bodyserver.cpp ${RestbedSRCS})
