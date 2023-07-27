
find_library(re2_LIBRARY re2 HINTS "/usr/local/re2/lib64" NO_DEFAULT_PATH)

find_path(re2_INCLUDE re2/re2.h HINTS "/usr/local/re2/include" NO_DEFAULT_PATH)

if(re2_LIBRARY AND re2_INCLUDE)
    set(RE2_FOUND TRUE)
    
    message(STATUS "${Green}Found re2 library at: ${re2_LIBRARY}${Reset}")
    message(STATUS "${Green}Found re2 include at: ${re2_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate re2 dependency.${Reset}")
endif()
