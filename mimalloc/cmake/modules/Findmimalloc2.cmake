
find_library(mimalloc2_LIBRARY mimalloc HINTS "/usr/local/mimalloc2/lib")

find_path(mimalloc2_INCLUDE mimalloc.h HINTS "/usr/local/mimalloc2/include/mimalloc-2.0")

if(mimalloc2_LIBRARY AND mimalloc2_INCLUDE)
    set(MIMALLOC2_FOUND TRUE)
    
    message(STATUS "${Green}Found mimalloc2 library at: ${mimalloc2_LIBRARY}${Reset}")
    message(STATUS "${Green}Found mimalloc2 include at: ${mimalloc2_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate mimalloc2 dependency.${Reset}")
endif()
