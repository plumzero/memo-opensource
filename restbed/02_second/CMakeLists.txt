
cmake_minimum_required(VERSION 2.8.10)

#
# POST
#
add_executable(postclient postclient.cpp ${RestbedSRCS})
add_executable(postserver postserver.cpp ${RestbedSRCS})