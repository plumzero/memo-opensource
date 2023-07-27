
#include <cstdio>
#include <cassert>
#include <string>

#include "trade.pb.h"

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    std::string strCancelOrder;
    {
        printf("========== 开始序列化测试 ==========\n");
        mam::CancelOrder cancel_order;
        
        cancel_order.set_reqid(12345);      
        cancel_order.set_orderid("order_" + std::to_string(12345));
        
        assert(cancel_order.SerializeToString(&strCancelOrder));
        
        printf("stream size=%lu\n", strCancelOrder.size());
    }   // end namespace

    {
        printf("========== 开始结构化测试 ==========\n");
        mam::CancelOrder cancel_order;
        
        assert(cancel_order.ParseFromString(strCancelOrder));
        
        printf("reqId: %d\n", cancel_order.reqid());
        printf("orderId: %s\n", cancel_order.orderid().c_str());        
    }   // end namespace
    
    return 0;
}
