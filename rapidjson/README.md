
## 对 rapidjson 的简单封装后测试

## 文件说明
- 封装后的文件为 raj.h ，其余为测试文件；
- 属于对 rapidjson 进行模板封装后的工具类文件；
- 支持 windows 和 linux；
- 已在 linux 下进行过内存泄漏测试，测试结果为无内存泄漏，测试命令如下:
  ```shell
    valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./测试程序
  ```
  
## 文件分类
- 封装测试文件  `bic.* rajtest.cpp`
- 纯粹 rapidjson 效率测试文件  
  
## 解析格式说明
- 不同的数据类型或结构，其 json 方式的表现可能有多种，即序列化方式可能有多种，所以:
  the parse method must corresponding to the write method defined previously.
  ```shell
    std::pair       object
    std::vector     array
    std::list       array
    std::set        array
    std::map        object
    std::multimap   array
    std::multiset   array
  ```

## 功能说明
- 可支持的常见基本类型与 rapidjson 对应，其中包括 bool、int、unsigned int、int64_t、uint64_t、double；
- 未实现 char、signed char、unsigned char、long、long double 等罕见基本类型，可使用常见类型代替；
- 支持 enum 类型，本质上是 int32_t；
- 支持 std::string 类型；
- 不支持 char*、unsigned char* 类型，可使用 std::string 代替；
- 未实现 std::list、std::set 容器，可使用 std::vector 代替；
- 未实现 std::multimap 容器，可使用 std::vector 和 std::pair 联合代替；
- 未实现 std::multiset 容器，可使用 std::vector 代替；
- 基本上，所有类型均可以通过在已定义类型方法基础上组合实现；
  但数据结构不支持类似如下的非典型结构("小" 值 "大" 键)，比如
  ```c++
    std::pair<std::pair<K, T>, V>
    std::pair<std::vector<T>, V>
    std::pair<std::map<K, T>, V>
    std::map<std::pair<K, T>, V>
    std::map<std::vector<T>, V>
    ...
  ```
  
## 其他使用说明
- rapidjson 使用用户缓存效果更好，而不是在堆中分配内存；
- [使用Document时内存问题](https://blog.csdn.net/guotianqing/article/details/99712689)
- [Memory leak - Parse #1333](https://github.com/Tencent/rapidjson/issues/1333)
