

### 备忘

如何将大量的数据落盘？或者说落盘时哪些问题应该值得关注呢？

我想值得关注的问题应该有两个。一是落盘数据要紧凑，这些才能占用更少的磁盘空间。二是落盘速度要快，也就是说，要尽量减少拷贝的次数。

接下来针对这两点进行简单的分析(猜测)。

> 如何使落盘数据更紧凑?

以数组方式进行数据的存储可以使占用更紧凑，考虑到数据是结构化的，可以尝试使用矩阵。矩阵在内存中的物理表现仍然是一维数组，如何使之"看起来"像矩阵呢？可以为存储格式添加一些元数据信息(metadata)。

> 如何使落盘更快?

...

### 编程注意

DataSpace::getSimpleExtentDims 函数很危险，慎用。使用时一般传入空指针。



### 测试文件说明

> 1-first   矩阵测试(展示)

> 2-second  数组测试(展示)

> 3-third   结构测试

> 4-fourth  结构中的字符串测试

> 5-fifth   结构中的数组测试

> 6-sixth   OOP测试