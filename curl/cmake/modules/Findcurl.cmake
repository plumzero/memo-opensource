
find_library(curl_LIBRARY curl HINTS "/usr/local/curl/lib" NO_DEFAULT_PATH)

find_path(curl_INCLUDE curl/curl.h HINTS "/usr/local/curl/include" NO_DEFAULT_PATH)

if(curl_LIBRARY AND curl_INCLUDE)
    set(curl_FOUND TRUE)
    
    message(STATUS "${Green}Found curl library at: ${curl_LIBRARY}${Reset}")
    message(STATUS "${Green}Found curl include at: ${curl_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate curl dependency.${Reset}")
endif()

get_filename_component(curl_LIBRARY_NAME ${curl_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" curl_LIBRARY_NAME ${curl_LIBRARY_NAME})
get_filename_component(curl_LIBRARY_PATH ${curl_LIBRARY} PATH)

message(STATUS "${Blue}curl_INCLUDE=${curl_INCLUDE}${Reset}")
message(STATUS "${Blue}curl_LIBRARY_PATH=${curl_LIBRARY_PATH}${Reset}")
