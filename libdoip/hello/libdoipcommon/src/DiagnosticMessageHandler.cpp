#include "DiagnosticMessageHandler.h"
#include <iostream>
#include <cstring>

/**
 * Checks if a received Diagnostic Message is valid
 * @param cb                    callback which will be called with the user data
 * @param sourceAddress		currently registered source address on the socket
 * @param data			message which was received
 * @param diagMessageLength     length of the diagnostic message
 */
// 检验接收到的诊断报文是否有效
// @param cb                用于处理此诊断报文用户数据的回调
// @param sourceAddress     当前注册到服务端套接字上的(客户端的)逻辑地址
// @param data              诊断报文载荷
unsigned char parseDiagnosticMessage(DiagnosticCallback callback, unsigned char sourceAddress [2],
                                    unsigned char* data, int diagMessageLength) {
    std::cout << "parse Diagnostic Message" << std::endl;
    if(diagMessageLength >= _DiagnosticMessageMinimumLength) {
        //Check if the received SA is registered on the socket
        if(data[0] != sourceAddress[0] || data[1] != sourceAddress[1]) {
            //SA of received message is not registered on this TCP_DATA socket
            return _InvalidSourceAddressCode;
        }

        std::cout << "source address valid" << std::endl;
        //Pass the diagnostic message to the target network/transport layer
        unsigned short target_address = 0;
        target_address |= ((unsigned short)data[2]) << 8U;
        target_address |= (unsigned short)data[3];

        int cb_message_length = diagMessageLength - _DiagnosticMessageMinimumLength;
        unsigned char* cb_message = new unsigned char[cb_message_length];

        for(int i = _DiagnosticMessageMinimumLength; i < diagMessageLength; i++) {  // 可以使用 memcpy 进行优化
            cb_message[i - _DiagnosticMessageMinimumLength] = data[i];
        }

        callback(target_address, cb_message, cb_message_length);

        //return positive ack code
        return _ValidDiagnosticMessageCode;
    }
    return _UnknownTargetAddressCode;
}

/**
 * Creates a diagnostic message positive/negative acknowledgment message
 * @param type                  defines positive/negative acknowledge type
 * @param sourceAddress		logical address of the receiver of the previous diagnostic message
 * @param targetAddress		logical address of the sender of the previous diagnostic message
 * @param responseCode		positive or negative acknowledge code
 * @return pointer to the created diagnostic message acknowledge
 */
// 创建一个诊断信息积极/消极响应消息
// @param type          定义哪种响应类型 积极/消极?
// @param sourceAddress 之前诊断报文中的接收者(也就是服务端)的逻辑地址
// @param targetAddress 之前诊断报文中的发送者(也就是客户端)的逻辑地址
// @param responseCode  积极或消极响应码(0x0 表示积极应答，见表15。非0x0表示消极应答，见表17)
// @return 指向创建好的诊断信息响应消息
unsigned char* createDiagnosticACK(bool ackType, unsigned short sourceAddress, 
                                    unsigned char targetAddress [2], unsigned char responseCode) {
    
    PayloadType type;
    if(ackType)
        type = PayloadType::DIAGNOSTICPOSITIVEACK;
    else
        type = PayloadType::DIAGNOSTICNEGATIVEACK;
    
    unsigned char* message = createGenericHeader(type, _DiagnosticPositiveACKLength);   // 无论诊断信息积极/消极响应，载荷长度不低于 5

    // 见 表14 和 表16
    //add source address to the message
    message[8] = (unsigned char)((sourceAddress >> 8) & 0xFF);
    message[9] = (unsigned char)(sourceAddress & 0xFF);

    //add target address to the message
    message[10] = targetAddress[0];
    message[11] = targetAddress[1];

    //add positive or negative acknowledge code to the message
    message[12] = responseCode;

    return message;
}

/**
 * Creates a complete diagnostic message
 * @param sourceAddress		logical address of the sender of a diagnostic message
 * @param targetAddress		logical address of the receiver of a diagnostic message
 * @param userData		actual diagnostic data
 * @param userDataLength	length of diagnostic data
 */
// 创建诊断报文
unsigned char* createDiagnosticMessage(unsigned short sourceAddress, unsigned char targetAddress [2],
                                        unsigned char* userData, int userDataLength) {
    
    unsigned char* message = createGenericHeader(PayloadType::DIAGNOSTICMESSAGE, _DiagnosticMessageMinimumLength + userDataLength);

    //add source address to the message
    message[8] = (unsigned char)((sourceAddress >> 8) & 0xFF);
    message[9] = (unsigned char)(sourceAddress & 0xFF);

    //add target address to the message
    message[10] = targetAddress[0];
    message[11] = targetAddress[1];

    //add userdata to the message
    for(int i = 0; i < userDataLength; i++) {   // memcpy 优化
        message[12 + i] = userData[i];
    }

    return message;
}
