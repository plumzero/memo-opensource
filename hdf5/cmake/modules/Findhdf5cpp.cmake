
find_library(hdf5cpp_LIBRARY hdf5_cpp "/usr/local/hdf5/lib")

find_path(hdf5cpp_INCLUDE H5Cpp.h HINTS "/usr/local/hdf5/include")

if(hdf5cpp_INCLUDE AND hdf5cpp_LIBRARY)
    set(HDF5CPP_FOUND TRUE)
    
    message(STATUS "${Green}Found hdf5cpp include at: ${hdf5cpp_INCLUDE}${Reset}")
    message(STATUS "${Green}Found hdf5cpp library at: ${hdf5cpp_LIBRARY}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate hdf5cpp dependency.${Reset}")
endif()
