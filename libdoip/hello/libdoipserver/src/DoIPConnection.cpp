#include "DoIPConnection.h"

#include <iostream>
#include <iomanip>

/**
 * Closes the connection by closing the sockets
 */
// 一旦出现超时情况，就会调用此函数。主要是两步，一是底层调用关闭套接字，二是应用层完成对资源的处理
void DoIPConnection::aliveCheckTimeout() {
    std::cout << "Alive Check Timeout. Close Connection" << std::endl;
    closeSocket();
    close_connection();
}

/*
 * Closes the socket for this server
 */
void DoIPConnection::closeSocket() {
    close(tcpSocket);
    tcpSocket = 0;
}

/*
 * Receives a message from the client and calls reactOnReceivedTcpMessage method
 * @return      amount of bytes which were send back to client
 *              or -1 if error occurred     
 */
// 阻塞接收来自客户端的消息并处理(包括检验、解析、发送应答、处理用户数据等)
int DoIPConnection::receiveTcpMessage() {
    std::cout << "Waiting for DoIP Header..." << std::endl;
    unsigned char genericHeader[_GenericHeaderLength];
    unsigned int readBytes = receiveFixedNumberOfBytesFromTCP(_GenericHeaderLength, genericHeader);
    if(readBytes == _GenericHeaderLength && !aliveCheckTimer.timeout) {
        std::cout << "Received DoIP Header." << std::endl;
        GenericHeaderAction doipHeaderAction = parseGenericHeader(genericHeader, _GenericHeaderLength);

        unsigned char *payload = nullptr;
        if(doipHeaderAction.payloadLength > 0) {
            std::cout << "Waiting for " << doipHeaderAction.payloadLength << " bytes of payload..." << std::endl;
            payload = new unsigned char[doipHeaderAction.payloadLength];
            unsigned int receivedPayloadBytes = receiveFixedNumberOfBytesFromTCP(doipHeaderAction.payloadLength, payload);
            if(receivedPayloadBytes != doipHeaderAction.payloadLength) {
                closeSocket();
                return 0;
            }
            std::cout << "DoIP message completely received" << std::endl;
        }

        //if alive check timouts should be possible, reset timer when message received(每接收一次消息，重置一次计时器)
        if(aliveCheckTimer.active) {
            aliveCheckTimer.resetTimer();
        }

        int sentBytes = reactOnReceivedTcpMessage(doipHeaderAction, doipHeaderAction.payloadLength, payload);
        
        return sentBytes;
    } else {
        closeSocket();
        return 0;
    }
    return -1;
      
}

/**
 * Receive exactly payloadLength bytes from the TCP stream and put them into receivedData.
 * The method blocks until receivedData bytes are received or the socket is closed.
 * 
 * The parameter receivedData needs to point to a readily allocated array with
 * at least payloadLength items.
 * 
 * @return number of bytes received
*/
// 以定长的方式从套接字中抽取数据
unsigned long DoIPConnection::receiveFixedNumberOfBytesFromTCP(unsigned long payloadLength, unsigned char *receivedData) {
    unsigned long payloadPos = 0;
    unsigned long remainingPayload = payloadLength;

    while(remainingPayload > 0) {
        int readBytes = recv(tcpSocket, &receivedData[payloadPos], remainingPayload, 0);
        if(readBytes <= 0) {
            return payloadPos;
        }
        payloadPos += readBytes;
        remainingPayload -= readBytes;
    }

    return payloadPos;
}

/*
 * Receives a message from the client and determine how to process the message
 * @return      amount of bytes which were send back to client
 *              or -1 if error occurred     
 */
// 对载荷的处理
int DoIPConnection::reactOnReceivedTcpMessage(GenericHeaderAction action, unsigned long payloadLength, unsigned char *payload) {

    std::cout << "processing DoIP message..." << std::endl;
    int sentBytes;
    switch(action.type) {
        // 对消极应答的处理(原因有很多)
        case PayloadType::NEGATIVEACK: {
            //send NACK
            sentBytes = sendNegativeAck(action.value);

            if(action.value == _IncorrectPatternFormatCode || 
                    action.value == _InvalidPayloadLengthCode) {
                closeSocket();
                return -1;
            }

            return sentBytes;
        }
        // 对路由激活请求的处理
        case PayloadType::ROUTINGACTIVATIONREQUEST: {
            //start routing activation handler with the received message
            unsigned char result = parseRoutingActivation(payload);
            unsigned char clientAddress [2] = {payload[0], payload[1]};
            // 将 sendMessage 放在对 result 的判断之前处理，表示无论结果如何，都要返给客户端一个响应
            unsigned char* message = createRoutingActivationResponse(logicalGatewayAddress, clientAddress, result);
            sentBytes = sendMessage(message, _GenericHeaderLength + _ActivationResponseLength);

            if(result == _UnknownSourceAddressCode || result == _UnsupportedRoutingTypeCode) {
                closeSocket();
                return -1;
            } else {
                //Routing Activation Request was successfull, save address of the client(路由激活请求成功后，保存客户端逻辑地址。可以看出服务端与客户端一一对应)
                routedClientAddress = new unsigned char[2];
                routedClientAddress[0] = payload[0];
                routedClientAddress[1] = payload[1];

                //start alive check timer(为这个客户端激活一个定时器，并绑定超时回调函数)
                if(!aliveCheckTimer.active) {
                    aliveCheckTimer.cb = std::bind(&DoIPConnection::aliveCheckTimeout,this);
                    aliveCheckTimer.startTimer();
                }
            }

            return sentBytes;
        }
        // 对活跃检查请求的处理(不进行处理)
        case PayloadType::ALIVECHECKRESPONSE: {
            return 0;
        }
        // 对诊断报文的处理
        case PayloadType::DIAGNOSTICMESSAGE: {
            // 见 表12 诊断报文载荷结构: [0,1]-诊断报文发送者逻辑地址 [2,3]-诊断报文接收者逻辑地址
            unsigned short target_address = 0;
            target_address |= ((unsigned short)payload[2]) << 8U;
            target_address |= (unsigned short)payload[3];
            bool ack = notify_application(target_address);  // 一旦接收到诊断报文，会立即通知接收者。接收者可以发送一个诊断消息积极/消极响应给客户端

            // 如果诊断消息响应是积极的(即诊断报文有效)，则进一步解析该诊断报文并处理用户数据
            if(ack)
                parseDiagnosticMessage(diag_callback, routedClientAddress, payload, payloadLength);

            break;
        }

        default: {
            std::cerr << "Received message with unhandled payload type: " << action.type << std::endl;
            return -1;
        }
    }  
    return -1;
}

void DoIPConnection::triggerDisconnection() {
    std::cout << "Application requested to disconnect Client from Server" << std::endl;
    closeSocket();
}

/**
 * Sends a message back to the connected client
 * @param message           contains generic header and payload specific content
 * @param messageLength     length of the complete message
 * @return                  number of bytes written is returned,
 *                          or -1 if error occurred
 */
int DoIPConnection::sendMessage(unsigned char* message, int messageLength) {
    int result = write(tcpSocket, message, messageLength);
    return result;
}

/**
 * Sets the time in seconds after which a alive check timeout occurs.
 * Alive check timeouts can be deactivated when setting the seconds to 0
 * @param seconds   time after which alive check timeout occurs
 */
// 为定时器设定一个超时时间(秒计)，当指定为 0 表示停用定时器
void DoIPConnection::setGeneralInactivityTime(uint16_t seconds) {
    if(seconds > 0) {
        aliveCheckTimer.setTimer(seconds);
    } else {
        aliveCheckTimer.disabled = true;
    }
}

/*
 * Send diagnostic message payload to the client
 * @param sourceAddress   logical source address (i.e. address of this server)
 * @param data     received payload
 * @param length    length of received payload
 */
// 创建并发送一个诊断报文给客户端
// @param sourceAddress 发送方(这里指服务端)的逻辑地址
// @param data          要发送的载荷
// @param length        要发送的载荷的长度
void DoIPConnection::sendDiagnosticPayload(unsigned short sourceAddress, unsigned char* data, int length) {

    std::cout << "Sending diagnostic data: ";
    for(int i = 0; i < length; i++) {
        std::cout << std::hex << std::setw(2) << (unsigned int)data[i] << " ";
    }
    std::cout << std::endl;
    
    unsigned char* message = createDiagnosticMessage(sourceAddress, routedClientAddress, data, length);  
    sendMessage(message, _GenericHeaderLength + _DiagnosticMessageMinimumLength + length);
}

/*
 * Set the callback function for this doip server instance
 * @dc      Callback which sends the data of a diagnostic message to the application
 * @dmn     Callback which notifies the application of receiving a diagnostic message
 * @ccb     Callback for application function when the library closes the connection
 */
void DoIPConnection::setCallback(DiagnosticCallback dc, DiagnosticMessageNotification dmn, CloseConnectionCallback ccb) {
    diag_callback = dc;
    notify_application = dmn;
    close_connection = ccb;
}

// 发送一个诊断响应信息
// sourceAddress 表示发送方(也就是服务端)逻辑地址，actType 为 true 表示一个积极响应码，...
void DoIPConnection::sendDiagnosticAck(unsigned short sourceAddress, bool ackType, unsigned char ackCode) {
    unsigned char data_TA [2] = { routedClientAddress[0], routedClientAddress[1] };
    // 创建诊断响应消息，发送给客户端
    unsigned char* message = createDiagnosticACK(ackType, sourceAddress, data_TA, ackCode);
    sendMessage(message, _GenericHeaderLength + _DiagnosticPositiveACKLength);
}

/**
 * Prepares a generic header nack and sends it to the client
 * @param ackCode       NACK-Code which will be included in the message
 * @return              amount of bytes sended to the client
 */
// 创建并发送一个消极应答 NACK
int DoIPConnection::sendNegativeAck(unsigned char ackCode) {
    unsigned char* message = createGenericHeader(PayloadType::NEGATIVEACK, _NACKLength);
    message[8] = ackCode;
    int sendedBytes = sendMessage(message, _GenericHeaderLength + _NACKLength);
    return sendedBytes;
}
