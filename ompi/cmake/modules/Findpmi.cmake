
find_library(pmi_LIBRARY mpi HINTS "/usr/local/ompi/lib" NO_DEFAULT_PATH)

find_path(pmi_INCLUDE mpi.h HINTS "/usr/local/ompi/include" NO_DEFAULT_PATH)

if(pmi_LIBRARY AND pmi_INCLUDE)
    set(PMI_FOUND TRUE)
    
    message(STATUS "${Green}Found pmi library at: ${pmi_LIBRARY}${Reset}")
    message(STATUS "${Green}Found pmi include at: ${pmi_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate pmi dependency.${Reset}")
endif()
