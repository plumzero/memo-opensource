
#include <stdio.h>
#include <re2/filtered_re2.h>
#include <re2/re2.h>

/**
 * 测试目的:
 *  1. Add 的使用
 *  2. Compile 的使用
 *  3. FirstMatch 的使用
 */

int main()
{
    re2::FilteredRE2 f;
    int id = 12;

    RE2::ErrorCode rc;

    // 添加模式匹配规则
    rc = f.Add("a.*b.*c", RE2::DefaultOptions, &id);
    if (rc != RE2::NoError) {
        printf("re2::FilteredRE2::Add failed: %d\n", rc);
        return -1;
    }
    printf("当前模式匹配规则个数: %d\n", id);

    // 添加另一个模式匹配规则
    rc = f.Add("d.*e.*f", RE2::DefaultOptions, &id);
    if (rc != RE2::NoError) {
        printf("re2::FilteredRE2::Add failed: %d\n", rc);
        return -1;
    }
    printf("当前模式匹配规则个数: %d\n", id);

    // 模式匹配规则重复添加
    rc = f.Add("a.*b.*c", RE2::DefaultOptions, &id);
    if (rc != RE2::NoError) {
        printf("re2::FilteredRE2::Add failed: %d\n", rc);
        return -1;
    }
    printf("当前模式匹配规则个数: %d\n", id);

    // 预处理
    std::vector<std::string> v;
    f.Compile(&v);
    printf("v.size=%lu\n", v.size());
    
    // 查找匹配规则进行匹配
    std::vector<int> ids;
    int ret = f.FirstMatch("deeff", ids);
    if (ret < 0) {
        printf("re2::FilteredRE2::FirstMatch failed: %d\n", ret);
        return -1;
    }

    printf("v.size=%lu\n", v.size());

    printf("ret=%d\n", ret);

    printf("====== test meet exception ======\n");

    return 0;
}
