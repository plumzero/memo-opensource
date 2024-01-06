
find_path(sqlite_INCLUDE sqlite3.h HINTS "${PROJECT_SOURCE_DIR}/sqlite/" NO_DEFAULT_PATH)

file(GLOB sqlite_SOURCES "${PROJECT_SOURCE_DIR}/sqlite/sqlite3.c")

if(sqlite_INCLUDE AND sqlite_SOURCES)
    set(SQLITE_FOUND TRUE)
    
    message(STATUS "${Green}Found sqlite include at: ${sqlite_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate sqlite dependency.${Reset}")
endif()
