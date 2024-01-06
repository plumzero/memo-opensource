
find_path(sqliteorm_INCLUDE sqlite_orm/sqlite_orm.h HINTS "${PROJECT_SOURCE_DIR}/orm/include" NO_DEFAULT_PATH)

if(sqliteorm_INCLUDE)
    set(SQLITEORM_FOUND TRUE)
    
    message(STATUS "${Green}Found sqliteorm include at: ${sqliteorm_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate sqliteorm dependency.${Reset}")
endif()
