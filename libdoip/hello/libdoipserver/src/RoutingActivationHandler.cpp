#include "RoutingActivationHandler.h"

/*
    表8 路由激活请求报文载荷
    +-------+-------+-------+--------------------------+---------------------+
    | 项目   | 位置  |  长度  |  描述                     |  值                 |
    +-------+-------+-------+--------------------------+---------------------+
    | 测试设备地址信息                                                          |
    +-------+-------+-------+--------------------------+---------------------+
    |源地址  | 0     | 2     |测试设备路由激活请求报文地址信息|                     |
    +-------+-------+-------+---------------------------+--------------------+
    |激活类型| 2     | 1     |路由激活的类型               |                     |
    +-------+-------+-------+--------------------------+---------------------+
    |预留信息                                                                 |
    +--------+------+-------+--------------------------+---------------------+
    |13400预留| 3    | 4     |                          | 0x00000000         |
    +--------+------+-------+--------------------------+---------------------+
    |整车厂预留| 7     | 4     |                          | 0x00000000          |
    +-------+-------+-------+--------------------------+---------------------+

 */

/**
 * Checks if the Routing Activation Request is valid
 * @param data  contains the request
 * @return      routing activation response code 
 */
unsigned char parseRoutingActivation(unsigned char *data) {
    
    //Check if source address is known(校验源地址)
    uint32_t address = 0;
    address |= (uint32_t)data[0] << 8;
    address |= (uint32_t)data[1];
    if(!checkSourceAddress(address)) {
        //send routing activation negative response code --> close socket
        return _UnknownSourceAddressCode;
    }
    
    //Check if routing activation type is supported(校验路由激活类型 表9)
    switch(data[2]) {
        case 0x00: {
            //Activation Type default
            break;
        }
        case 0x01: {
            //Activation Type WWH-OBD
            break;
        }
        default: {
            //unknown activation type
            //send routing activation negative response code --> close socket
            return _UnsupportedRoutingTypeCode;
        }
    }
    
    //if not exited before, send routing activation positive response
    return _SuccessfullyRoutedCode;
}

/**
 * Create the complete routing activation response, which also contains the
 * generic header
 * @param clientAddress     address of the test equipment
 * @param responseCode      routing activation response code // 比如 _UnsupportedRoutingTypeCode 等
 * @return                  complete routing activation response
 */
// sourceAddress 为服务端(也即调用该方法创建路由激活响应发送一方)逻辑地址
// clientAddress 为服务端(也即测试设备逻辑地址)
unsigned char* createRoutingActivationResponse(unsigned short sourceAddress, unsigned char clientAddress[2],
                                                unsigned char responseCode) {
    
    unsigned char* message = createGenericHeader(PayloadType::ROUTINGACTIVATIONRESPONSE,
                                                    _ActivationResponseLength);
    // 表10
    //Logical address of external test equipment (测试设备逻辑地址)
    message[8] = clientAddress[0];
    message[9] = clientAddress[1];
    
    //logical address of DoIP entity (DoIP主体逻辑地址)
    message[10] = (unsigned char)((sourceAddress >> 8) & 0xFF);
    message[11] = (unsigned char)(sourceAddress & 0xFF);
    
    //routing activation response code (路由激活响应值)
    message[12] = responseCode;
    
    //reserved for future standardization use
    message[13] = 0x00;
    message[14] = 0x00;
    message[15] = 0x00;
    message[16] = 0x00;           
    
    return message;
}

/**
 * Checks if the submitted address is valid
 * @param address	the address which will be checked
 * @return			true if address is valid
 */
bool checkSourceAddress(uint32_t address) {
    uint32_t minAddress = 3584;  // 0x0E00
    uint32_t maxAddress = 4095;     // 0x0FFF
    
    for(uint32_t i = minAddress; i <= maxAddress; i++) {
        if(address == i) {
            return true;
        }
    }
           
    return false;
}