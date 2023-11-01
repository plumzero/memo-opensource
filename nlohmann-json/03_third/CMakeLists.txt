
cmake_minimum_required(VERSION 3.5)

file(COPY my.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY array.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(file ${files})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file})
endforeach()
