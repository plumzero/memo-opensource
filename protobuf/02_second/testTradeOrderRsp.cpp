
#include <cstdio>
#include <cassert>
#include <string>
#include <utility>

#include "trade.pb.h"

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    std::string strTradeOrderRsp;
    {
        printf("========== 开始序列化测试 ==========\n");
        mam::TradeOrderRsp trade_order_rsp;
        mam::OrderMatchResult * order_match_result;
        mam::OrderChangeResult * order_change_result;
        
        order_match_result = trade_order_rsp.add_matches();
        order_match_result->set_reqid(123);
        order_match_result->set_orderid("order_" + std::to_string(123));
        order_match_result->set_matchid("order_over_" + std::to_string(123));
        order_match_result->set_time(1600396786);
        order_match_result->set_dealprice(1200);
        order_match_result->set_dealvolume(798);
        
        order_match_result = trade_order_rsp.add_matches();
        order_match_result->set_reqid(456);
        order_match_result->set_orderid("order_" + std::to_string(456));
        order_match_result->set_matchid("order_over_" + std::to_string(456));
        order_match_result->set_time(1600396866);
        order_match_result->set_dealprice(1100);
        order_match_result->set_dealvolume(384);
        
        order_match_result = trade_order_rsp.add_matches();
        order_match_result->set_reqid(789);
        order_match_result->set_orderid("order_" + std::to_string(789));
        order_match_result->set_matchid("order_over_" + std::to_string(789));
        order_match_result->set_time(1600396908);
        order_match_result->set_dealprice(-1400);
        order_match_result->set_dealvolume(124);
        
        order_change_result = trade_order_rsp.add_status();
        order_change_result->set_reqid(123);
        order_change_result->set_orderid("order_" + std::to_string(123));
        order_change_result->set_time(1600397212);
        order_change_result->set_status(mam::OrderStatusType::OrderStatus_TotalMatch);
        order_change_result->set_errcode(mam::TradeOrderErrorCode::JErrorType_StopTrade);
        order_change_result->set_errdetail("Stop Trade");
        
        order_change_result = trade_order_rsp.add_status();
        order_change_result->set_reqid(456);
        order_change_result->set_orderid("order_" + std::to_string(456));
        order_change_result->set_time(1600397392);
        order_change_result->set_status(mam::OrderStatusType::OrderStatus_Accepted);
        order_change_result->set_errcode(mam::TradeOrderErrorCode::JErrorType_TooHighFrequency);
        order_change_result->set_errdetail("Too High Frequency");
        
        assert(trade_order_rsp.SerializeToString(&strTradeOrderRsp));
        
        printf("stream size=%lu\n", strTradeOrderRsp.size());
    }   // end namespace

    {
        printf("========== 开始结构化测试 ==========\n");
        mam::TradeOrderRsp trade_order_rsp;
        
        assert(trade_order_rsp.ParseFromString(strTradeOrderRsp));
        
        decltype(std::declval<mam::TradeOrderRsp>().matches_size()) i;
        for (i = 0; i < trade_order_rsp.matches_size(); i++) {
            const mam::OrderMatchResult & order_match_result = trade_order_rsp.matches(i);
            printf("reqId=%d ", order_match_result.reqid());
            printf("orderId=%s ", order_match_result.orderid().c_str());
            printf("matchId=%s ", order_match_result.matchid().c_str());
            printf("time=%u ", order_match_result.time());
            printf("dealPrice=%d ", order_match_result.dealprice());
            printf("dealVolume=%d\n", order_match_result.dealvolume());
        }
        
        printf("\n");
        
        decltype(std::declval<mam::TradeOrderRsp>().status_size()) j;
        for (j = 0; j < trade_order_rsp.status_size(); j++) {
            const mam::OrderChangeResult & order_change_result = trade_order_rsp.status(j);
            printf("reqId=%d ", order_change_result.reqid());
            printf("orderId=%s ", order_change_result.orderid().c_str());
            printf("time=%u ", order_change_result.time());
            printf("status=%d ", order_change_result.status());
            printf("errCode=%d ", order_change_result.errcode());
            printf("errDetail=%s\n", order_change_result.errdetail().c_str());
        }
    }   // end namespace
    
    return 0;
}
