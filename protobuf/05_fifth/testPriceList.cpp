
#include <cstdio>
#include <cassert>
#include <string>
#include <algorithm>

#include "mtable.pb.h"

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    std::string strPriceList;
    {
        printf("========== 开始序列化测试 ==========\n");
        mam::PriceList price_list;
        mam::PriceObj price_obj;
        
        price_list.set_date("2021-01-15");
    
        price_obj.set_price("1023.456");
        price_list.mutable_dic()->insert(google::protobuf::MapPair<std::string, mam::PriceObj>("apple", price_obj));

        price_obj.set_price("2266.895");
        price_list.mutable_dic()->insert(google::protobuf::MapPair<std::string, mam::PriceObj>("pear", price_obj));
        
        assert(price_list.SerializeToString(&strPriceList));
        
        printf("stream size=%lu\n", strPriceList.size());
    }
    
    {
        printf("========== 开始结构化测试 ==========\n");
        mam::PriceList price_list;
        
        assert(price_list.ParseFromString(strPriceList));
        
        printf("date=%s\n", price_list.date().c_str());
        
        decltype(price_list.dic().begin()) it;
        for (it = price_list.dic().begin(); it != price_list.dic().end(); it++) {
            printf("key=%s, value=%s\n", it->first.c_str(), it->second.price().c_str());
        }
    }
    
    {
        // printf("========== 开始结构化测试 ==========\n");
        // mam::AccountInfos account_infos;
        
        // assert(account_infos.ParseFromString(strAccountInfos));
        
        // decltype(account_infos.accounts().begin()) it;
        // for (it = account_infos.accounts().begin(); it != account_infos.accounts().end(); it++) {
            // printf("accountid=%d host=%s\n", it->accountid(), it->host().c_str());
        // }
        
        // it = std::find_if(account_infos.accounts().begin(), account_infos.accounts().end(), [](decltype(*account_infos.accounts().begin())& ele){
            // return ele.accountid() == 199;
        // });
        // assert(it != account_infos.accounts().end());
        // printf("Find 199! Its host is %s\n", it->host().c_str());
    }   // end namespace
    
    return 0;
}
