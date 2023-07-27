
#include <fstream>
#include <cstdio>
#include <cassert>
#include <string>
#include <utility>

#include "fifteenth.pb.h"

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    // 无填充
    mam::TradeOrder trade_order;
    printf("sizeof(mam::TradeOrder): %ld|sizeof(trade_order): %ld\n", sizeof(mam::TradeOrder), sizeof(trade_order));

    // 填充基本类型后
    trade_order.set_reqid(12345);
    trade_order.set_symbol("Hello World");
    trade_order.set_timeout(3.1415926);
    trade_order.set_force(true);
    trade_order.set_createtime(1600393860);
    trade_order.set_status(mam::OrderStatusType::OrderStatus_Accepted);
    printf("sizeof(mam::TradeOrder): %ld|sizeof(trade_order): %ld\n", sizeof(mam::TradeOrder), sizeof(trade_order));

    // 填充复杂结构后
    trade_order.mutable_phone()->set_number("13657891234");
    trade_order.mutable_phone()->set_type(10086);
    printf("sizeof(mam::TradeOrder): %ld|sizeof(trade_order): %ld\n", sizeof(mam::TradeOrder), sizeof(trade_order));

    // 填充数组结构后
    trade_order.mutable_tags()->Add("One");
    trade_order.mutable_tags()->Add("Two");
    printf("tags size: %d\n", trade_order.tags_size());

    auto phone1 = trade_order.add_phones();
    phone1->set_number("13657894321");
    phone1->set_type(10086);
    auto phone2 = trade_order.add_phones();
    phone2->set_number("13657896543");
    phone2->set_type(10086);
    printf("phones size: %d\n", trade_order.phones_size());
    printf("sizeof(mam::TradeOrder): %ld|sizeof(trade_order): %ld\n", sizeof(mam::TradeOrder), sizeof(trade_order));
    
    // 填充字典结构后
    trade_order.mutable_dic()->insert(google::protobuf::MapPair<std::string, double>("Pi", 3.14159));
    trade_order.mutable_dic()->insert(google::protobuf::MapPair<std::string, double>("Golden", 0.618));
    trade_order.mutable_dic()->insert(google::protobuf::MapPair<std::string, double>("e", 2.718281829));
    printf("dic size: %d\n", trade_order.dic_size());
    
    mam::PhoneNumber phone_number1;
    phone_number1.set_number("13878945632");
    phone_number1.set_type(1234);
    trade_order.mutable_phonemap()->insert(google::protobuf::MapPair<std::string, mam::PhoneNumber>("yidong", phone_number1));
    mam::PhoneNumber phone_number2;
    phone_number2.set_number("13978945632");
    phone_number2.set_type(114);
    trade_order.mutable_phonemap()->insert(google::protobuf::MapPair<std::string, mam::PhoneNumber>("liantong", phone_number2));
    printf("phoneMap size: %d\n", trade_order.phonemap_size());
    printf("sizeof(mam::TradeOrder): %ld|sizeof(trade_order): %ld\n", sizeof(mam::TradeOrder), sizeof(trade_order));

    // printf("\n%s\n", trade_order.DebugString().c_str()); 

    return 0;
}