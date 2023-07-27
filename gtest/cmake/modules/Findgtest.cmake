
find_library(gtest_LIBRARY gtest HINTS "/usr/local/gtest/lib" NO_DEFAULT_PATH)
find_library(gtest_main_LIBRARY gtest_main HINTS "/usr/local/gtest/lib" NO_DEFAULT_PATH)
find_library(gmock_LIBRARY gmock HINTS "/usr/local/gtest/lib" NO_DEFAULT_PATH)
find_library(gmock_main_LIBRARY gmock_main HINTS "/usr/local/gtest/lib" NO_DEFAULT_PATH)

find_path(gtest_INCLUDE gtest/gtest.h HINTS "/usr/local/gtest/include" NO_DEFAULT_PATH)

if(gtest_LIBRARY AND gtest_main_LIBRARY AND gmock_LIBRARY AND gmock_main_LIBRARY AND gtest_INCLUDE)
    set(gtest_FOUND TRUE)
    
    message(STATUS "${Green}Found gtest library at: ${gtest_LIBRARY}${Reset}")
    message(STATUS "${Green}Found gtest_main library at: ${gtest_main_LIBRARY}${Reset}")
    message(STATUS "${Green}Found gmock library at: ${gmock_LIBRARY}${Reset}")
    message(STATUS "${Green}Found gmock_main library at: ${gmock_main_LIBRARY}${Reset}")
    message(STATUS "${Green}Found gtest include at: ${gtest_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate gtest dependency.${Reset}")
endif()

get_filename_component(gtest_LIBRARY_NAME ${gtest_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" gtest_LIBRARY_NAME ${gtest_LIBRARY_NAME})
get_filename_component(gtest_LIBRARY_PATH ${gtest_LIBRARY} PATH)

get_filename_component(gtest_main_LIBRARY_NAME ${gtest_main_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" gtest_main_LIBRARY_NAME ${gtest_main_LIBRARY_NAME})
get_filename_component(gtest_main_LIBRARY_PATH ${gtest_main_LIBRARY} PATH)

get_filename_component(gmock_LIBRARY_NAME ${gmock_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" gmock_LIBRARY_NAME ${gmock_LIBRARY_NAME})
get_filename_component(gmock_LIBRARY_PATH ${gmock_LIBRARY} PATH)

get_filename_component(gmock_main_LIBRARY_NAME ${gmock_main_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" gmock_main_LIBRARY_NAME ${gmock_main_LIBRARY_NAME})
get_filename_component(gmock_main_LIBRARY_PATH ${gmock_main_LIBRARY} PATH)

message(STATUS "${Blue}gtest_INCLUDE=${gtest_INCLUDE}${Reset}")
message(STATUS "${Blue}gtest_LIBRARY_PATH=${gtest_LIBRARY_PATH}${Reset}")
message(STATUS "${Blue}gtest_main_LIBRARY_PATH=${gtest_main_LIBRARY_PATH}${Reset}")
message(STATUS "${Blue}gmock_LIBRARY_PATH=${gmock_LIBRARY_PATH}${Reset}")
message(STATUS "${Blue}gmock_main_LIBRARY_PATH=${gmock_main_LIBRARY_PATH}${Reset}")
