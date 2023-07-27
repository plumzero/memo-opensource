cmake_minimum_required(VERSION 2.8.0)

#
# protoc --cpp_out=. arenatest.proto
#

# 关于 arena ，在 <C专家编程> 中说道:
# "有些人用 arena 这个术语描述由内存分配器(memory allocator)管理的内存块的集合(在 SunOS 中，就是从当前 break 的位置到数据段结尾之间的区域)。"

include_directories(.)

add_executable(testThirteenth testThirteenth.cpp arenatest.pb.cc)
target_link_libraries(testThirteenth ${protobuf_LIBRARY_NAME})