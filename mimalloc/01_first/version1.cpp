
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

#include "mimalloc.h"
#include "struct.pb.h"
#include "utils.h"

/*
    测试数据(毫秒)

    默认: 16/16/16
    mim: 16/16/16

    效率也没提多少~
 */

int main()
{
    const size_t sz = 2 * 1024 * 1024;      // 2M 的长度
    const int total = 40960;                // 要修改的次数

    using Type = mam::TradeOrder;

    std::cout << "元素长度: " << sizeof(Type) << " 分配内存: " << sz * sizeof(Type) << std::endl;

    // 缺省分配器
    {
        std::vector<Type> vec;
        vec.reserve(sz * sizeof(Type));

        for (size_t i = 0; i < sz; i++) {
            vec.emplace_back(Type());
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
        auto b1 = std::chrono::steady_clock::now();

        for (int j = 0; j < total; j++) {
            int index = ((sz / total) * j * 97) % sz;
            vec[index].set_reqid(j);
            vec[index].set_symbol(std::to_string(j));
            vec[index].set_orderprice(j * 1000);
            vec[index].set_ordervolume(j * 100);
            vec[index].set_force(j % 2);
            vec[index].set_createtime(j * 1000);
            vec[index].set_closetime(j * 10000 + j * 100);
            vec[index].set_status(mam::OrderStatus_TotalMatch);
            vec[index].set_errcode(mam::JErrorType_TooHighFrequency);
        }

        auto e1 = std::chrono::steady_clock::now();
        std::cout << "缺省分配器耗时(毫秒): " << timediff<std::chrono::milliseconds>(b1, e1) << std::endl;
    }

    // mimalloc 分配器
    {
        std::vector<Type, mi_stl_allocator<Type>> vec;
        vec.reserve(sz * sizeof(Type));

        for (size_t i = 0; i < sz; i++) {
            vec.emplace_back(Type());
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
        auto b1 = std::chrono::steady_clock::now();

        for (int j = 0; j < total; j++) {
            int index = ((sz / total) * j * 97) % sz;
            vec[index].set_reqid(j);
            vec[index].set_symbol(std::to_string(j));
            vec[index].set_orderprice(j * 1000);
            vec[index].set_ordervolume(j * 100);
            vec[index].set_force(j % 2);
            vec[index].set_createtime(j * 1000);
            vec[index].set_closetime(j * 10000 + j * 100);
            vec[index].set_status(mam::OrderStatus_TotalMatch);
            vec[index].set_errcode(mam::JErrorType_TooHighFrequency);
        }

        auto e1 = std::chrono::steady_clock::now();
        std::cout << "mimalloc耗时(毫秒): " << timediff<std::chrono::milliseconds>(b1, e1) << std::endl;
    }

    std::cout << "=== over ===" << std::endl;

    return 0;
}