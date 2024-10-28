
在进行仓库改造时，在 Debug 时出现如下错误，但在 Release 时却不会报错。

Debug 编译时报错如下:
```s
[100%] Linking CXX executable bm_lane_det
/usr/bin/ld: /path/to/lib/libpitch_est.so: undefined reference to `google::base::CheckOpMessageBuilder::CheckOpMessageBuilder(char const*)'
/usr/bin/ld: /path/to/lib/libpitch_est.so: undefined reference to `google::base::CheckOpMessageBuilder::~CheckOpMessageBuilder()'
/usr/bin/ld: /path/to/lib/libpitch_est.so: undefined reference to `google::base::CheckOpMessageBuilder::ForVar2()'
/usr/bin/ld: /path/to/lib/libpitch_est.so: undefined reference to `google::LogMessage::stream()'
/usr/bin/ld: /path/to/lib/libpitch_est.so: undefined reference to `google::LogMessageFatal::LogMessageFatal(char const*, int, google::CheckOpString const&)'
/usr/bin/ld: /path/to/lib/libpitch_est.so: undefined reference to `google::LogMessageFatal::~LogMessageFatal()'
/usr/bin/ld: /path/to/lib/libpitch_est.so: undefined reference to `google::base::CheckOpMessageBuilder::NewString[abi:cxx11]()'
/usr/bin/ld: /path/to/lib/libpitch_est.so: undefined reference to `google::LogMessageFatal::LogMessageFatal(char const*, int)'
collect2: error: ld returned 1 exit status
```
大概意思是在生成可执行程序 bm_lane_det 时需要链接到 libpitch_est.so 库，而这个库里面的一些符号没有找到定义。

找到 libpitch_est.so Debug 库，查看符号:
```s
nm libpitch_est.so | grep CheckOpMessageBuilder
00000000003c6476 W _ZN6google4base21CheckOpMessageBuilder7ForVar1Ev
                 U _ZN6google4base21CheckOpMessageBuilder7ForVar2Ev
                 U _ZN6google4base21CheckOpMessageBuilder9NewStringB5cxx11Ev
                 U _ZN6google4base21CheckOpMessageBuilderC1EPKc
                 U _ZN6google4base21CheckOpMessageBuilderD1Ev
```
但是 libpitch_est.so Release 库却没有这些。

在生成 libpitch_est.so 的工程中，链接到了 ceres 库，而这个库中有使用到了上面这些符号。

但在我在编译 Ceres 的时候已经使用了 `-DMINIGLOG=ON` 选项，它表示不使用 glog 。

而且，在编译 libpitch_est.so 的工程中，我也已经定义了寻找 minilog 的 find*.cmake 文件，如下:
```s

set(miniglog_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/build_depend/${PLATFORM_TYPE}-${BUILD_TYPE}/include/ceres/internal/miniglog")

if (miniglog_INCLUDE_DIRS)
  set(miniglog_FOUND TRUE)
  include_directories(${miniglog_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "${Red}Failed to locate miniglog module.${Reset}")
endif()
```
另外，我还定义了寻找 ceres 的 find*.cmake 文件。

而且，在一些路径下面，确实有 glog 库，虽然我没有显式为它们定义 find*.cmake 文件，但通过某些方式确实可能找到了它们。

我原来是先寻找 miniglog 再寻找 ceres 的，如下:
```s
include(${PROJECT_SOURCE_DIR}/cmake/modules/FindCeres.cmake)
...
include(${PROJECT_SOURCE_DIR}/cmake/modules/Findminiglog.cmake)
```
是不是应该优先寻找 miniglog 呢，我调换了一下:
```s
include(${PROJECT_SOURCE_DIR}/cmake/modules/Findminiglog.cmake)
...
include(${PROJECT_SOURCE_DIR}/cmake/modules/FindCeres.cmake)
```
可以了。
