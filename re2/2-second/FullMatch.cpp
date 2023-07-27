
#include <stdio.h>
#include <re2/filtered_re2.h>
#include <re2/re2.h>

int main()
{
    {
        bool bl = RE2::FullMatch("axbyc", "a.*b.*c");
        if (! bl) {
            printf("RE2::FullMatch failed\n");
            return -1;
        }
    }

    {
        // 不符合全匹配
        int n;
        bool bl = RE2::FullMatch("foo123bar", "(\\d+)", &n);
        if (bl) {
            printf("RE2::FullMatch failed\n");
        }
        printf("%s is not match with regular expression '(\\d+)'\n", "foo123bar");
    }

    {
        int n;
        bool bl = RE2::FullMatch("foo123bar", "foo(\\d+)bar", &n);
        if (! bl) {
            printf("RE2::FullMatch failed\n");
            return -1;
        }
        printf("捕获串存储为整型: %d\n", n);
    }

    {
        double d;
        bool bl = RE2::FullMatch("foo123.456bar", "foo(\\d+\\.\\d+)bar", &d);
        if (! bl) {
            printf("RE2::FullMatch failed\n");
            return -1;
        }
        printf("捕获串存储为浮点型: %f\n", d);
    }

    {
        std::string str;
        bool bl = RE2::FullMatch("foo123bar", "foo(\\d+)bar", &str);
        if (! bl) {
            printf("RE2::FullMatch failed\n");
            return -1;
        }
        printf("捕获串存储为字符串: %s\n", str.c_str());
    }

    {
        // 如果觉得转义麻烦的话，可以使用字符串纸面值
        const std::string regexp = R"(foo(\d+)bar)";
        std::string str;
        bool bl = RE2::FullMatch("foo123bar", regexp, &str);
        if (! bl) {
            printf("RE2::FullMatch failed\n");
            return -1;
        }
        printf("模式串采用纸面值匹配: %s\n", str.c_str());
    }

    {
        // 用处不大
        re2::StringPiece sp;
        bool bl = RE2::FullMatch("foo123bar", "foo(\\d+)bar", &sp);
        if (! bl) {
            printf("RE2::FullMatch failed\n");
            return -1;
        }
        printf("捕获串存储为 StringPiece: %s\n", sp.as_string().c_str());
    }

    {
        std::string str1, str2, str3;
        bool bl = RE2::FullMatch("foo123bar456buzz0", "foo(\\d+)bar(\\d+)(\\w+)", &str1, &str2, &str3);
        if (! bl) {
            printf("RE2::FullMatch failed\n");
            return -1;
        }
        printf("多个捕获之分别存储: %s %s %s\n", str1.c_str(), str2.c_str(), str3.c_str());
    }

    {
        std::string str[3];
        bool bl = RE2::FullMatch("foo123bar456buzz0", "foo(\\d+)bar(\\d+)(\\w+)", &str[0], &str[1], &str[2]);
        if (! bl) {
            printf("RE2::FullMatch failed\n");
            return -1;
        }
        printf("多个捕获之数组存储: %s %s %s\n", str[0].c_str(), str[1].c_str(), str[2].c_str());
    }

    {
        int i, j;
        std::string str;
        bool bl = RE2::FullMatch("foo123bar456buzz0", "foo(\\d+)bar(\\d+)(\\w+)", &i, &j, &str);
        if (! bl) {
            printf("RE2::FullMatch failed\n");
            return -1;
        }
        printf("多个捕获之混合类型存储: %d %d %s\n", i, j, str.c_str());
    }

    printf("====== test meet exception ======\n");

    return 0;
}
