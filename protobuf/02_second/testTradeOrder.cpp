
#include <cstdio>
#include <cassert>
#include <string>

#include "trade.pb.h"

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    std::string strTradeOrder;
    {
        printf("========== 开始序列化测试 ==========\n");
        mam::TradeOrder trade_order;
        
        trade_order.set_reqid(12345);
        trade_order.set_orderid("order_" + std::to_string(12345));
        trade_order.set_symbol(std::to_string(60000) + ".SH");
        trade_order.set_ordertype(1);
        trade_order.set_tradetype(1);
        trade_order.set_orderprice(25000);
        trade_order.set_ordervolume(-1000);
        trade_order.set_ntime(1533289490);
        trade_order.set_accountid(188);
        trade_order.set_tag("tag_tradeorder");
        trade_order.set_timeout(5000);
        trade_order.set_force(true);
        trade_order.set_createtime(1600393860);
        trade_order.set_submittime(1600393882);
        trade_order.set_accepttime(1600393893);
        trade_order.set_closetime(1600393905);
        trade_order.set_dealprice(30000);
        trade_order.set_dealvolume(1600393916);
        trade_order.set_status(mam::OrderStatusType::OrderStatus_Accepted);
        trade_order.set_errcode(mam::TradeOrderErrorCode::JErrorType_None);
        trade_order.set_errdetail("No Error");
        
        assert(trade_order.SerializeToString(&strTradeOrder));
        
        printf("stream size=%lu\n", strTradeOrder.size());
    }   // end namespace

    {
        printf("========== 开始结构化测试 ==========\n");
        mam::TradeOrder trade_order;
        
        assert(trade_order.ParseFromString(strTradeOrder));
        
        printf("reqId: %d\n", trade_order.reqid());
        printf("orderId: %s\n", trade_order.orderid().c_str());
        printf("symbol: %s\n", trade_order.symbol().c_str());
        printf("orderType: %d\n", trade_order.ordertype());
        printf("orderPrice: %d\n", trade_order.orderprice());
        printf("orderVolume: %d\n", trade_order.ordervolume());
        printf("nTime: %d\n", trade_order.ntime());
        printf("accountId: %d\n", trade_order.accountid());
        printf("tag: %s\n", trade_order.tag().c_str());
        printf("timeout: %d\n", trade_order.timeout());
        printf("force: %d\n", trade_order.force());
        printf("createTime: %ld\n", trade_order.createtime());
        printf("submitTime: %ld\n", trade_order.submittime());
        printf("acceptTime: %ld\n", trade_order.accepttime());
        printf("closeTime: %ld\n", trade_order.closetime());
        printf("dealPrice: %d\n", trade_order.dealprice());
        printf("dealVolume: %d\n", trade_order.dealvolume());
        printf("status: %d\n", trade_order.status());
        printf("errCode: %d\n", trade_order.errcode());
        printf("errDetail: %s\n", trade_order.errdetail().c_str());
        
    }   // end namespace
    
    return 0;
}
