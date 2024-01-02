
#ifndef DOIPCLIENT_H
#define DOIPCLIENT_H
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "DiagnosticMessageHandler.h"
#include "DoIPGenericHeaderHandler.h"

const int _serverPortNr=13400;
const int _maxDataSize=1024;


class DoIPClient{
    
public:
    void startTcpConnection(const char* ipAddr = "127.0.0.1");   
    void startUdpConnection(int udp_port = 13401);
    void sendRoutingActivationRequest();
    void sendVehicleIdentificationRequest(const char* address);
    void receiveUdpMessage();
    std::vector<uint8_t> receiveMessage();
    void sendDiagnosticMessage(unsigned char* targetAddress, unsigned char* userData, int userDataLength);
    void sendAliveCheckResponse();
    void setSourceAddress(unsigned char* address);
    void displayVIResponseInformation();
    void closeTcpConnection();
    void closeUdpConnection();
    void reconnectServer();

    int getSockFd();
    int getConnected();
    unsigned char* getLogicalAddress() { return LogicalAddressResult; }
    
private:
    unsigned char _receivedData[_maxDataSize];
    int _sockFd, _sockFd_udp, _connected;
    int broadcast = 1;
    struct sockaddr_in _serverAddr, _clientAddr; 
    unsigned char sourceAddress [2];
    
    unsigned char VINResult [17]{0};
    unsigned char LogicalAddressResult [2];
    unsigned char EIDResult [6];
    unsigned char GIDResult [6];
    unsigned char FurtherActionReqResult;
    
    const std::pair<int, unsigned char*>* buildRoutingActivationRequest();
    const std::pair<int, unsigned char*>* buildVehicleIdentificationRequest();
    void parseVIResponseInformation(unsigned char* data);
    
    int emptyMessageCounter = 0;
};



#endif /* DOIPCLIENT_H */

