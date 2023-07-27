
cmake_minimum_required(VERSION 3.0.2)

#
# Executable
#

add_executable(pgsql_connect pgsql_connect.cpp)
target_link_libraries(pgsql_connect ${pgsql_LIBRARY_NAME} ${pqxx_LIBRARY_NAME})

add_executable(pgsql_create pgsql_create.cpp)
target_link_libraries(pgsql_create ${pgsql_LIBRARY_NAME} ${pqxx_LIBRARY_NAME})

add_executable(pgsql_insert pgsql_insert.cpp)
target_link_libraries(pgsql_insert ${pgsql_LIBRARY_NAME} ${pqxx_LIBRARY_NAME})

add_executable(pgsql_update pgsql_update.cpp)
target_link_libraries(pgsql_update ${pgsql_LIBRARY_NAME} ${pqxx_LIBRARY_NAME})

add_executable(pgsql_select pgsql_select.cpp)
target_link_libraries(pgsql_select ${pqxx_LIBRARY_NAME} ${pgsql_LIBRARY_NAME})

add_executable(pgsql_delete pgsql_delete.cpp)
target_link_libraries(pgsql_delete ${pgsql_LIBRARY_NAME} ${pqxx_LIBRARY_NAME})

