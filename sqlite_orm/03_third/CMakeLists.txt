
add_executable(students students.cpp ${sqlite_SOURCES})
target_link_libraries(students -ldl -lpthread)
