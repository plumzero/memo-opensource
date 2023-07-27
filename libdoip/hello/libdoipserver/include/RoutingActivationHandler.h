#ifndef ROUTINGACTIVATIONHANDLER_H
#define ROUTINGACTIVATIONHANDLER_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include "DoIPGenericHeaderHandler.h"

// 路由激活响应报文长度，这里并没有算上[整车厂预留]的的 4 个长度(表10)
const int _ActivationResponseLength = 9;

// 路由激活响应值(表 11)
const unsigned char _UnknownSourceAddressCode = 0x00;
const unsigned char _UnsupportedRoutingTypeCode = 0x06;
const unsigned char _SuccessfullyRoutedCode = 0x10;

unsigned char parseRoutingActivation(unsigned char* data);
unsigned char* createRoutingActivationResponse(unsigned short sourceAddress, 
                                                unsigned char clientAddress[2],
                                                unsigned char responseCode);
bool checkSourceAddress(uint32_t address);


#endif /* ROUTINGACTIVATIONHANDLER_H */

