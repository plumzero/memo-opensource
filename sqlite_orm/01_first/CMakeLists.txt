
add_executable(sqlite_connect sqlite_connect.cpp ${sqlite_SOURCES})
target_link_libraries(sqlite_connect -ldl)

add_executable(sqlite_create sqlite_create.cpp ${sqlite_SOURCES})
target_link_libraries(sqlite_create -ldl)

add_executable(sqlite_insert sqlite_insert.cpp ${sqlite_SOURCES})
target_link_libraries(sqlite_insert -ldl)
