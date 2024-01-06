
add_executable(select select.cpp ${sqlite_SOURCES})
target_link_libraries(select -ldl -lpthread)

add_executable(update update.cpp ${sqlite_SOURCES})
target_link_libraries(update -ldl -lpthread)

add_executable(insert insert.cpp ${sqlite_SOURCES})
target_link_libraries(insert -ldl -lpthread)

add_executable(delete delete.cpp ${sqlite_SOURCES})
target_link_libraries(delete -ldl -lpthread)
