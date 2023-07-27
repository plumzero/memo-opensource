
find_library(hdf5_LIBRARY hdf5 HINTS "/usr/local/hdf5/lib/")

find_path(hdf5_INCLUDE hdf5.h HINTS "/usr/local/hdf5/include")

if(hdf5_LIBRARY AND hdf5_INCLUDE)
    set(HDF5_FOUND TRUE)
    
    message(STATUS "${Green}Found hdf5 library at: ${hdf5_LIBRARY}${Reset}")
    message(STATUS "${Green}Found hdf5 include at: ${hdf5_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate hdf5 dependency.${Reset}")
endif()

