
find_library(crypto_LIBRARY crypto HINTS "/usr/local/openssl/lib" NO_DEFAULT_PATH)

find_path(crypto_INCLUDE openssl/aes.h HINTS "/usr/local/openssl/include" NO_DEFAULT_PATH)

if(crypto_LIBRARY AND crypto_INCLUDE)
    set(CRYPTO_FOUND TRUE)
    
    message(STATUS "${Green}Found crypto library at: ${crypto_LIBRARY}${Reset}")
    message(STATUS "${Green}Found crypto include at: ${crypto_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate crypto dependency.${Reset}")
endif()
