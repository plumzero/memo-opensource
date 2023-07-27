
FilteredRE2 类接口。


### Add

```c++
    RE2::ErrorCode Add(const StringPiece& pattern,
                       const RE2::Options& options,
                       int* id);
```

参数:
```
    pattern    [IN]     模式匹配规则
    options    [IN]     一般使用 DefaultOptions
    id         [OUT]    当前 RE2 对象个数
```

描述: 为模式匹配规则创建正则表达式对象(regexp)并添加入正则集中。可以重复创建某个模式匹配规则的对象并添加。

返回: 成功时返回 NoError 。

### Compile

```c++
    void Compile(std::vector<std::string>* strings_to_match);
```

参数:
```
    strings_to_match    [OUT]       用于目标串校验的模式匹配规则(待理解)。
```

描述: 预处理正则集。

### FirstMatch

```c++
  int FirstMatch(const StringPiece& text,
                 const std::vector<int>& atoms) const;
```

参数:
```
    text    [IN]    目标串
    atoms   [IN]    
```

描述: 对目标串匹配，返回正则集中第一个符合要求的模式匹配规则的索引。

返回: 失败时返回 -1， 成功时返回第一个符合匹配的模式匹配规则的索引(从 0 开始)。


[示例程序参考](../1-first/FirstMatch.cpp)
