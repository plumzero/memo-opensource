#include "DoIPGenericHeaderHandler.h"
#include <iostream>
#include <iomanip>

using namespace std;

/*
    表4 通用 DoIP 数据报头结构
    +-------+-------+-------+---------------+--------------------------------+
    | 条目   | 位置  |  长度  |  描述          |  值                            |
    +-------+-------+-------+---------------+--------------------------------+
    | 通用报文头版本信息                                                        |
    +-------+-------+-------+---------------+--------------------------------+
    |协议版本| 0     | 1     |DoIP协议包版本   |0x02: ISO-13400-2:2012, 其他: 预留|
    +-------+-------+-------+---------------+--------------------------------+
    |版本反码| 1     | 1     |DoIP协议包版本反码|0xFD: 版本反码, 其他: 预留         |
    +-------+-------+-------+---------------+--------------------------------+
    |通用报文头载荷类型、载荷长度                                                 |
    +-------+-------+-------+-------------------------------------+----------+
    |载荷类型| 2     | 2     |如何解析通用DoIP报文头之后数据的标识符     | 见表5     |
    +-------+-------+-------+-------------------------------------+--------+-+
    |载荷长度| 4     | 4     |DoIP报文载荷长度。有一些载荷类型不需要附加参数，此时为0| |
    +-------+-------+-------+----------------------------------------------+-+

    表5 载荷类型  载荷类型值为16进制
    +--------+----------------+-------+-------+-------------------------------------------------+
    |载荷类型值|载荷类型         |DoIP网关|DoIP节点|端口/协议                                         |
    +--------+----------------+-------+-------+-------------------------------------------------+
    |0000    |DoIP首部否定响应  | M     | M     |UDP_DISCOVERY/UDP_TEST_EQUIPMENT_REQUEST/TCP_DATA|
    +--------+----------------+-------+-------+-------------------------------------------------+
    |0001    |车辆信息请求报文   | M     | M     |UDP_DISCOVERY |
    +--------+----------------+-------+-------+--------------+
    |0004    |车辆信息应该报文   | M     | M     |UDP_DISCOVERY |
    +--------+----------------+-------+-------+--------------+
    |0005    |路由激活请求      | M     | M     |TCP_DATA      |
    +--------+----------------+-------+-------+--------------+
    |0006    |路由激活应答      | M     | M     |TCP_DATA      |
    +--------+----------------+-------+-------+--------------+
    |0007    |活跃检查请求      | M     | M     |TCP_DATA      |
    +--------+----------------+-------+-------+--------------+
    |0008    |活跃检查应答      | M     | M     |TCP_DATA      |
    +--------+----------------+-------+-------+--------------+
    |8001    |诊断报文         | M     | M     |TCP_DATA      |
    +--------+----------------+-------+-------+--------------+
    |8002    |诊断积极应答报文   | M     | M     |TCP_DATA      |
    +--------+----------------+-------+-------+--------------+
    |8003    |诊断消极应答报文   | M     | M     |TCP_DATA      |
    +--------+----------------+-------+-------+--------------+
 */

/**
 * Checks if the received Generic Header is valid
 * @param data          message which was received
 * @param dataLenght    length of the message
 * @return              Returns a GenericHeaderAction struct, which stores the
 *                      payload type and a byte for further message processing
 */
GenericHeaderAction parseGenericHeader(unsigned char* data, int dataLenght) {
    
    GenericHeaderAction action;

    //Only check header if received data is greater or equals the set header length
    if(dataLenght >= _GenericHeaderLength) {
        
        //Check Generic DoIP synchronization pattern(校验协议包版本及版本反码)
        if((int)(data[1] ^ (0xFF)) != (int)data[0]) {
            //Return Error, Protocol Version not correct
            action.type = PayloadType::NEGATIVEACK;
            action.value = _IncorrectPatternFormatCode;
            action.payloadLength = 0;
            return action;
        }
        
        // 计算载荷长度
        unsigned int payloadLength = 0;
        payloadLength |= (unsigned int)(data[4] << 24);
        payloadLength |= (unsigned int)(data[5] << 16);
        payloadLength |= (unsigned int)(data[6] <<  8);
        payloadLength |= (unsigned int)(data[7] <<  0);

        action.payloadLength = payloadLength;

        //Check Payload Type(校验载荷类型)(未实现 0006路由激活应答/0007活跃检查请求/0008活跃检查应答)
        PayloadType messagePayloadType;
        if(data[2] == 0x00 && data[3] == 0x05) {	//Value of RoutingActivationRequest = 0x0005
            messagePayloadType = PayloadType::ROUTINGACTIVATIONREQUEST; // 路由激活请求
        }
        else if(data[2] == 0x00 && data[3] == 0x04){
            messagePayloadType = PayloadType::VEHICLEIDENTRESPONSE; // 车辆信息应答报文
        }
        else if(data[2] == 0x00 && data[3] == 0x01) {   //Value of Vehicle Identification Request = 0x0001
            messagePayloadType = PayloadType::VEHICLEIDENTREQUEST; // 车辆信息请求报文
        }
        else if(data[2] == 0x80 && data[3] == 0x01) {   //Value of Diagnose Message = 0x8001
            messagePayloadType = PayloadType::DIAGNOSTICMESSAGE; // 诊断报文
        } 
        else if(data[2] == 0x80 && data[3] == 0x02) {   //Value of Diagnostic Message positive ack = 0x8002
            messagePayloadType = PayloadType::DIAGNOSTICPOSITIVEACK; // 诊断积极应答报文
        } 
        else if(data[2] == 0x80 && data[3] == 0x03) {   //Value of Diagnostic Message negative ack = 0x8003
            messagePayloadType = PayloadType::DIAGNOSTICNEGATIVEACK; // 诊断消极应答报文
        } else {
            //Unknown Payload Type --> Send Generic DoIP Header NACK
            action.type = PayloadType::NEGATIVEACK;
            action.value = _UnknownPayloadTypeCode;
            return action;
        }

        //Check Payload Type specific length
        switch(messagePayloadType) {
            case PayloadType::ROUTINGACTIVATIONREQUEST: {
                if(payloadLength != 7 && payloadLength != 11) {
                    action.type = PayloadType::NEGATIVEACK;
                    action.value = _InvalidPayloadLengthCode;
                    return action;
                }
                break;
            }

            case PayloadType::ALIVECHECKRESPONSE: {
                if(payloadLength != 2) {
                    action.type = PayloadType::NEGATIVEACK;
                    action.value = _InvalidPayloadLengthCode;
                    return action;
                }
                break;
            }
            
            case PayloadType::VEHICLEIDENTREQUEST: {
                if(payloadLength != 0) {
                    action.type = PayloadType::NEGATIVEACK;
                    action.value = _InvalidPayloadLengthCode;
                    return action;
                }
                break;
            }

            case PayloadType::VEHICLEIDENTRESPONSE:{
                if(payloadLength != 32 && payloadLength != 33) {
                    action.type = PayloadType::NEGATIVEACK;
                    action.value = _InvalidPayloadLengthCode;
                    return action;
                }
                break;
            }

            case PayloadType::DIAGNOSTICMESSAGE: {
                if(payloadLength <= 4) {
                    action.type = PayloadType::NEGATIVEACK;
                    action.value = _InvalidPayloadLengthCode;
                    return action;
                }
                break;	
            }

            case PayloadType::DIAGNOSTICPOSITIVEACK: {
                if(payloadLength < 5) {
                    action.type = PayloadType::NEGATIVEACK;
                    action.value = _InvalidPayloadLengthCode;
                }
                break;
            }

            case PayloadType::DIAGNOSTICNEGATIVEACK: {
                if(payloadLength < 5) {
                    action.type = PayloadType::NEGATIVEACK;
                    action.value = _InvalidPayloadLengthCode;
                }
                break;
            }

            default: {
                std::cerr << "not handled payload type occured in parseGenericHeader()" << std::endl;
                break;	
            }
        }
        action.type = messagePayloadType;
    }
    
    return action;
}

/**
 * Creates a generic header
 * @param type      payload type which will be filled in the header
 * @param length    length of the payload type specific message
 * @return          header array
 */
unsigned char* createGenericHeader(PayloadType type, uint32_t length) {
    unsigned char *header = new unsigned char[8 + length];  // 通用报头长度 + 载荷长度
    header[0] = 0x02;
    header[1] = 0xFD;
    switch(type) {
        case PayloadType::ROUTINGACTIVATIONRESPONSE: {
            header[2] = 0x00;
            header[3] = 0x06;
            break;
        }
			
        case PayloadType::NEGATIVEACK: {
            header[2] = 0x00;
            header[3] = 0x00;
            break;
        }

        case PayloadType::VEHICLEIDENTRESPONSE:{
            header[2] = 0x00;
            header[3] = 0x04;
            break;
        }

        case PayloadType::DIAGNOSTICMESSAGE: {
            header[2] = 0x80;
            header[3] = 0x01;
            break;
        }

        case PayloadType::DIAGNOSTICPOSITIVEACK: {
            header[2] = 0x80;
            header[3] = 0x02;
            break;
        }

        case PayloadType::DIAGNOSTICNEGATIVEACK: {
            header[2] = 0x80;
            header[3] = 0x03;
            break;
        }
        
        case PayloadType::ALIVECHECKRESPONSE: {
            header[2] = 0x00;
            header[3] = 0x08;
            break;
        }

        default: {
            std::cerr << "not handled payload type occured in createGenericHeader()" << std::endl;
            break;
        }
    }
    
    header[4] = (length >> 24) & 0xFF;
    header[5] = (length >> 16) & 0xFF;
    header[6] = (length >> 8) & 0xFF;
    header[7] = length & 0xFF;
    
    return header;
}
