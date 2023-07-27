
#include "DoIPSimulator.h"
#include <functional>
#include <chrono>
#include <iomanip>

// 配置 Server
void DoIPSimulator::Configure()
{
  server_->setVIN("FOOBAR");
  server_->setLogicalGatewayAddress(LOGICAL_ADDRESS);
  server_->setGID(0);
  server_->setFAR(0);
  server_->setEID(0);
}

void DoIPSimulator::Run()
{
  server_active_ = true;
  
  doip_receiver_.push_back(std::thread([this]{ listenTcp(); }));
  doip_receiver_.push_back(std::thread([this]{ listenUdp(); }));
  
  std::this_thread::sleep_for(std::chrono::seconds(2));

  server_->sendVehicleAnnouncement();

  for (auto & th : doip_receiver_) {
    th.join();
  }
}

// 处理 TCP 消息
void DoIPSimulator::listenTcp() {
  server_->setupTcpSocket();

  while(true) {
    connection_ = server_->waitForTcpConnection();
    auto diag_cb = std::bind(&DoIPSimulator::ReceiveFromLibrary, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    auto notify_cb = std::bind(&DoIPSimulator::DiagnosticMessageReceived, this, std::placeholders::_1);
    auto close_cb = std::bind(&DoIPSimulator::CloseConnection, this);
    connection_->setCallback(diag_cb, notify_cb, close_cb);
    connection_->setGeneralInactivityTime(50000);

    while(connection_->isSocketActive()) {
      connection_->receiveTcpMessage();
    }
  }
}

// 处理 UDP 消息
void DoIPSimulator::listenUdp() {
  server_->setupUdpSocket();

  while(server_active_) {
    server_->receiveUdpMessage();
  }
}

// 当 doip 库接收到一个诊断消息时，会触发此回调
// @param address   ECU 的逻辑地址
// @param data      收到的消息
// @param length    收到的消息的长度
void DoIPSimulator::ReceiveFromLibrary(unsigned short address, unsigned char* data, int length) {
  std::cout << "DoIP Message received from 0x" << std::hex << address << ": ";
  for(int i = 0; i < length; i++) {
    std::cout << std::hex << std::setw(2) << (int)data[i] << " ";
  }
  std::cout << std::endl;

  if(length > 2 && data[0] == 0x22)  {
    std::cout << "-> Send diagnostic message positive response" << std::endl;
    unsigned char responseData[] = { 0x62, data[1], data[2], 0x01, 0x02, 0x03, 0x04};
    connection_->sendDiagnosticPayload(LOGICAL_ADDRESS, responseData, sizeof(responseData));
  } else {
    std::cout << "-> Send diagnostic message negative response" << std::endl;
    unsigned char responseData[] = { 0x7F, data[0], 0x11};
    connection_->sendDiagnosticPayload(LOGICAL_ADDRESS, responseData, sizeof(responseData));
  }
}

// 在接收到一个诊断消息时将进行此回调，库将通知应用这个消息，检查是否有关于这个逻辑地址的 ECU
// @param  targetAddress 到 ECU 的逻辑地址
// @return 发送给客户端的积极/消极ACK
bool DoIPSimulator::DiagnosticMessageReceived(unsigned short targetAddress) {
  (void)targetAddress;
  unsigned char ackCode;

  std::cout << "Received Diagnostic message" << std::endl;

  //send positiv ack
  ackCode = 0x00;
  std::cout << "-> Send positive diagnostic message ack" << std::endl;
  connection_->sendDiagnosticAck(LOGICAL_ADDRESS, true, ackCode);

  return true;
}

/**
 * Closes the connection of the server by ending the listener threads
 */
void DoIPSimulator::CloseConnection() {
  std::cout << "Connection closed" << std::endl;
  //serverActive = false;
}