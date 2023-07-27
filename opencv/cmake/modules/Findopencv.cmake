
find_library(opencv_LIBRARY opencv_core HINTS "/usr/local/opencv/lib" NO_DEFAULT_PATH)

find_path(opencv_INCLUDE opencv2/core.hpp HINTS "/usr/local/opencv/include/opencv4" NO_DEFAULT_PATH)

if(opencv_LIBRARY AND opencv_INCLUDE)
    set(OPENCV_FOUND TRUE)
    
    message(STATUS "${Green}Found opencv library at: ${opencv_LIBRARY}${Reset}")
    message(STATUS "${Green}Found opencv include at: ${opencv_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate opencv dependency.${Reset}")
endif()
