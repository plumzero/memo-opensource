
cmake_minimum_required(VERSION 2.8.10)

#
# efficiency of POST method
#
add_executable(efficlient efficlient.cpp ${RestbedSRCS})
add_executable(effiserver effiserver.cpp ${RestbedSRCS})
