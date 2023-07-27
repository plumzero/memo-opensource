
add_executable(RoutingActivationHandler_Test RoutingActivationHandler_Test.cpp ${LIBDOIP_SRCS})
target_link_libraries(RoutingActivationHandler_Test PUBLIC gtest gtest_main pthread)

add_executable(DoIPGenericHeaderHandler_Test DoIPGenericHeaderHandler_Test.cpp ${LIBDOIP_SRCS})
target_link_libraries(DoIPGenericHeaderHandler_Test PUBLIC gtest gtest_main pthread)

add_executable(Server Server.cpp ${LIBDOIP_SRCS} ${REALIZE_SRCS})
target_link_libraries(Server PUBLIC pthread)

add_executable(Client Client.cpp ${LIBDOIP_SRCS})
target_link_libraries(Client PUBLIC pthread)

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/0*.cpp")
foreach(file ${files})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file} ${LIBDOIP_SRCS})
    target_link_libraries(${name} PUBLIC pthread)
endforeach()
