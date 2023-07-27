
全局接口。

### FullMatch

```c++
    template <typename... A>
        static bool FullMatch(const StringPiece& text, const RE2& re, A&&... a);
```

参数:
```
    text    [IN]        目标串
    re      [IN]        模式匹配规则
    a       [OUT][OP]   捕获到的匹配子串的存储类型。可以是 std::string, StringPiece, T(根据捕获串的类型), NULL(表示不存储)等，但不支持数组或映射存储。
```

描述: 对目标串进行全匹配。

返回: 匹配成功返回 true, 匹配失败返回 false 。

[示例程序](../2-second/FullMatch.cpp)


### PartialMatch

```c++
    template <typename... A>
        static bool PartialMatch(const StringPiece& text, const RE2& re, A&&... a) {
            return Apply(PartialMatchN, text, re, Arg(std::forward<A>(a))...);
        }
```

参数: 参考 FullMatch 。

描述: 目标串中是否有符合模式匹配规则的部分。较之 FullMatch 更为常用。

返回: 参考 FullMatch 。

[示例程序](../3-third/PartialMatch.cpp)
