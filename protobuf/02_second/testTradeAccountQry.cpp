
#include <cstdio>
#include <cassert>
#include <string>

#include "trade.pb.h"

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    std::string strTradeAccountQry;
    {
        printf("========== 开始序列化测试 ==========\n");
        mam::TradeAccountQry trade_account_qry;
        
        trade_account_qry.set_accountid(188);      
        
        assert(trade_account_qry.SerializeToString(&strTradeAccountQry));
        
        printf("stream size=%lu\n", strTradeAccountQry.size());
    }   // end namespace

    {
        printf("========== 开始结构化测试 ==========\n");
        mam::TradeAccountQry trade_account_qry;
        
        assert(trade_account_qry.ParseFromString(strTradeAccountQry));
        
        printf("reqId: %d\n", trade_account_qry.accountid());
    }   // end namespace
    
    return 0;
}
