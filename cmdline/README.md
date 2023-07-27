


### 参数释义

```cpp
    a.add<int>("port", 'p', "port number", false, 80, cmdline::range(1, 65535));
```

1. 长参数，如 --port
2. 短参数，如 -p
3. 参数含义
4. 是否为必选项，true 为必选，false 为可选
5. 缺省参数
6. 参数值范围限制
