
find_library(mimalloc1_LIBRARY mimalloc HINTS "/usr/local/mimalloc1/lib")

find_path(mimalloc1_INCLUDE mimalloc.h HINTS "/usr/local/mimalloc1/include/mimalloc-1.7")

if(mimalloc1_LIBRARY AND mimalloc1_INCLUDE)
    set(MIMALLOC1_FOUND TRUE)
    
    message(STATUS "${Green}Found mimalloc1 library at: ${mimalloc1_LIBRARY}${Reset}")
    message(STATUS "${Green}Found mimalloc1 include at: ${mimalloc1_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate mimalloc1 dependency.${Reset}")
endif()
