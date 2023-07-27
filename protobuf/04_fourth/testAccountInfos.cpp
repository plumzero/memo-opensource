
#include <cstdio>
#include <cassert>
#include <string>
#include <algorithm>

#include "trade.pb.h"

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    std::string strAccountInfos;
    {
        printf("========== 开始序列化测试 ==========\n");
        mam::AccountInfos account_infos;
        mam::TradeAccountRsp * trade_account_rsp;
        
        trade_account_rsp = account_infos.add_accounts();
        trade_account_rsp->set_accountid(188);
        trade_account_rsp->set_host("136.25.42.78");
        
        trade_account_rsp = account_infos.add_accounts();
        trade_account_rsp->set_accountid(199);
        trade_account_rsp->set_host("192.168.10.78");
        
        assert(account_infos.SerializeToString(&strAccountInfos));
        
        printf("stream size=%lu\n", strAccountInfos.size());
    }   // end namespace

    {
        printf("========== 开始结构化测试 ==========\n");
        mam::AccountInfos account_infos;
        
        assert(account_infos.ParseFromString(strAccountInfos));
        
        decltype(account_infos.accounts().begin()) it;
        for (it = account_infos.accounts().begin(); it != account_infos.accounts().end(); it++) {
            printf("accountid=%d host=%s\n", it->accountid(), it->host().c_str());
        }
        
        it = std::find_if(account_infos.accounts().begin(), account_infos.accounts().end(), [](decltype(*account_infos.accounts().begin())& ele){
            return ele.accountid() == 199;
        });
        assert(it != account_infos.accounts().end());
        printf("Find 199! Its host is %s\n", it->host().c_str());
    }   // end namespace
    
    return 0;
}
