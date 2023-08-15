
HDiffPatch 是一个高效的 diff/patch 实现,比 bsdiff 更快(平均1/7时间),占用的内存更小(1/2内存),更容易使用和集成,得到的 diff 结果压缩后也经常比 bsdiff 更小或相当(一般小10%以上)! 

测试版本为 v4.6.3。

HDiffPatch 依赖的库有 zlib 和 bzip2(默认会使用系统自带库)，原始的 HDiffPatch 采用 Makefile 方式编译。这里将其调整为 CMake 编译，并将 zlib(v1.2.13) 和 bzip2(1.0.6) 源码加入，使可以独立编译。

HDiffPatch 使用很简单(HDiffPatch支持直接压缩补丁包，patch的时候不用提前解压):
- create diff(newData字节数组,oldData字节数组,out diffData): 生成了 oldData 到 newData 的差异信息 diffData。
- patch(out newData,oldData,diffData): 用 oldData 和 diffData 就可以合成 newData。

根据新包和旧包生成差分包:
```s
  ./hdiffz -s old.zip new.zip patch
```
根据旧包和差分包产生新包:
```s
  ./hpatchz -s-4m old.zip patch out.zip
```
