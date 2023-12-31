#ifndef DOIPCONNECTION_H
#define DOIPCONNECTION_H

#include <iostream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <net/if.h>
#include <unistd.h>
#include "DoIPGenericHeaderHandler.h"
#include "RoutingActivationHandler.h"
#include "VehicleIdentificationHandler.h"
#include "DoIPGenericHeaderHandler.h"
#include "RoutingActivationHandler.h"
#include "DiagnosticMessageHandler.h"
#include "AliveCheckTimer.h"

using CloseConnectionCallback = std::function<void()>;

const unsigned long _MaxDataSize = 0xFFFFFF;

class DoIPConnection {

public:
    DoIPConnection(int tcpSocket, unsigned short logicalGatewayAddress): 
        tcpSocket(tcpSocket), logicalGatewayAddress(logicalGatewayAddress) { };
    
    int receiveTcpMessage();
    unsigned long receiveFixedNumberOfBytesFromTCP(unsigned long payloadLength, unsigned char *receivedData);

    void sendDiagnosticPayload(unsigned short sourceAddress, unsigned char* data, const int length);
    bool isSocketActive() { return tcpSocket != 0; };

    void triggerDisconnection();
    
    void sendDiagnosticAck(unsigned short sourceAddress, bool ackType, unsigned char ackCode);
    int sendNegativeAck(unsigned char ackCode);

    void setCallback(DiagnosticCallback dc, DiagnosticMessageNotification dmn, CloseConnectionCallback ccb);                       
    void setGeneralInactivityTime(const uint16_t seconds);   

private:

    int tcpSocket;

    AliveCheckTimer aliveCheckTimer;
    DiagnosticCallback diag_callback;
    CloseConnectionCallback close_connection;
    DiagnosticMessageNotification notify_application;

    unsigned char* routedClientAddress{nullptr}; // 保存的客户端的逻辑地址
    unsigned short logicalGatewayAddress = 0x0000;  // 服务端(本端)逻辑地址
        
    void closeSocket();

    int reactOnReceivedTcpMessage(GenericHeaderAction action, unsigned long payloadLength, unsigned char *payload);
    
    int sendMessage(unsigned char* message, int messageLenght);
    
    void aliveCheckTimeout();
};

#endif /* DOIPCONNECTION_H */
