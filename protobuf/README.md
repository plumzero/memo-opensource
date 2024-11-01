
## 测试版本

- 当前安装及测试版本 `3.13.0`


## 安装步骤

方式一:

```s
  ./autogen.sh
  ./configure --prefix=/usr/local/protobuf
  make
  make install
```

这种方式在有的平台上可能会有些问题，可以使用[这个autogen.sh](autogen.sh)替换原始的同名文件，并将 Makefile.am 中的:
```s
DIST_SUBDIRS = src conformance benchmarks third_party/googletest
```
改为:
```s
DIST_SUBDIRS = src conformance benchmarks
```

方式二:

```s
  tar -zxvf protobuf-3.13.0.tar.gz
  cd protobuf-3.13.0
  cd cmake
  mkdir build
  cd build
  cmake .. -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local/protobuf
  make
  make install
```

## 利用 protoc 命令创建 .cpp 文件

- 安装完成后，在 `/usr/local/protobuf/bin` 下会有产生一个可执行命令 `protoc`。将该命令添加软链接到系统执行路径:
  ```shell
    ln -s /usr/local/protobuf/bin/protoc /usr/bin/protoc
  ```
- 创建关于 `.proto` 文件的 `.h` 和 `.cc` 文件
  ```shell
    protoc --cpp_out=. addressbook.proto
  ```
  如果 `addressbook.proto` 内部还依赖其他文件，可以通过 `-I` 或 `--proto_path` 指定依赖文件所在的最深层目录。


## 测试目录说明

- 01_first: 基本使用
- 02_second: 基本使用
- 03_third: mutable_xxx 使用
- 04_forth: 通过迭代器访问 repeated 结构
- 05_fifth: 映射表的使用
- 06_sixth: 类型描述
- 07_seventh: 类型反射(alpha)
- 08_eighth: 类型反射(beta)
- 09_ninth: option 使用(alpha)
- 10_tenth: option 使用(beta)
- 11_eleventh: 反射方式向 pb 结构的 repeated 字段添加元素
- 12_twelfth: 反射方式向 pb 结构的 map 字段中添加元素
- 13_thirteenth: arena 测试
- 14_fourteenth: FieldOptions 测试
- 15_fifteenth: 结构长度
- 16_sixteenth: 引用使用