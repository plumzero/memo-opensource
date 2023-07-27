
#include <cstdio>
#include <cstdlib>
#include <string>
#include <tuple>
#include <vector>
#include <utility>

#include "sixteenth.pb.h"

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    auto givenOrderId = [] () {
        int i, n;
        n = rand() % 5 + 4;
        std::string str;
        str.resize(n);
        for(i = 0; i < n; ++i)
        {
            str[i] = 'a' + rand() % 26;
        }
        
        return str;
    };

    std::vector<std::tuple<std::string, int32_t, int32_t>> tuples;
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 200));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 500));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 500));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 800));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 1600));
    tuples.push_back(std::make_tuple(givenOrderId(), 49500, 200));

    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 5900));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 1600));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 1100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 1200));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 2400));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 800));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 300));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 2000));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 1100));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 1200));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 1400));
    tuples.push_back(std::make_tuple(givenOrderId(), 49600, 5300));

    std::vector<mam::OrderMatchResult> matches;

    int id = 0;
    for (auto t : tuples) {
        mam::OrderMatchResult match;
        printf("%ld: %s,%d,%d\n", sizeof(match), std::get<0>(t).c_str(), std::get<1>(t), std::get<2>(t));
        match.set_reqid(id++);
        match.set_orderid(std::get<0>(t));
        match.set_dealprice(std::get<1>(t));
        match.set_dealvolume(std::get<2>(t));
        
        matches.push_back(match);
    }

    printf("\nmatches size: %ld\n", matches.size());

    for (size_t i = 0; i < matches.size(); i++) {
        printf("sizeof(matches[%ld]): %ld\n", i, sizeof(matches[i]));
    }

    // 修改索引为 16 的元素
    printf("原始(%ld): %s\n", matches.size(), matches[16].ShortDebugString().c_str());
    auto ele = matches[16];
    ele.set_dealprice(49700);
    ele.set_dealvolume(2000);
    printf("赋值(%ld): %s\n", matches.size(), matches[16].ShortDebugString().c_str());  // 没有变化

    // 指针
    auto ptr = &matches[16];
    ptr->set_dealprice(49800);
    ptr->set_dealvolume(1800);
    printf("指针(%ld): %s\n", matches.size(), matches[16].ShortDebugString().c_str());  // 变化了

    auto ref = std::move(matches[16]);
    ref.set_dealprice(49800);
    ref.set_dealvolume(1800);
    printf("引用(%ld): %s\n", matches.size(), matches[16].ShortDebugString().c_str());  // 无了

    return 0;
}