
#include <stdio.h>
#include <re2/filtered_re2.h>
#include <re2/re2.h>

int main()
{
    {
        bool bl = RE2::PartialMatch("axbyc", "a.*b.*c");
        if (! bl) {
            printf("RE2::PartialMatch failed\n");
            return -1;
        }
    }

    {
        // 符合部分匹配
        int n;
        bool bl = RE2::PartialMatch("foo123bar", "(\\d+)", &n);
        if (! bl) {
            printf("RE2::PartialMatch failed\n");
            return -1;
        }

        printf("匹配值为: %d\n", n);
    }

    {
        // 空不匹配
        bool bl = RE2::PartialMatch("", "[\\S]{1,}");
        if (! bl) {
            printf("null meet demand\n");
        } else {
            printf("failed\n");
            return -1;
        }

        bl = RE2::PartialMatch("   ", "[\\S]{1,}");
        if (! bl) {
            printf("tab meet demand\n");
        } else {
            printf("failed\n");
            return -1;
        }

        bl = RE2::PartialMatch("a ", "[\\S]{1,}");
        if (bl) {
            printf("[a ] meet demand\n");
        } else {
            printf("failed\n");
            return -1;
        }

        bl = RE2::PartialMatch("   bc", "[\\S]{1,}");
        if (bl) {
            printf("[   bc] meet demand\n");
        } else {
            printf("failed\n");
            return -1;
        }

        bl = RE2::PartialMatch(" ef  ", "[\\S]{1,}");
        if (bl) {
            printf("[   ef  ] meet demand\n");
        } else {
            printf("failed\n");
            return -1;
        }

        bl = RE2::PartialMatch("ghi", "[\\S]{1,}");
        if (bl) {
            printf("[ghi] meet demand\n");
        } else {
            printf("failed\n");
            return -1;
        }
    }

    printf("====== test meet exception ======\n");

    return 0;
}
