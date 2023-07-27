
版本 `v1.1.0` 示例

### 编译安装

```cmake
    # fetch third-party dependencies
    git submodule init
    git submodule update

    mkdir build
    cd build

    cmake .. -DCMAKE_INSTALL_PREFIX=/tmp/prometheus-cpp -DBUILD_SHARED_LIBS=ON -DENABLE_PUSH=ON -DENABLE_COMPRESSION=OFF
    
    # build
    cmake --build . --parallel 4
    
    # run tests
    ctest -V

    # install the libraries and headers
    cmake --install .
```
