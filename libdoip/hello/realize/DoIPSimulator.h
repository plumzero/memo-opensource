
#pragma once

#include "DoIPServer.h"
#include <vector>
#include <thread>
#include <memory>
#include <iostream>

static const unsigned short LOGICAL_ADDRESS = 0x28;

class DoIPSimulator {
public:
  DoIPSimulator() : server_(new DoIPServer) {}
  ~DoIPSimulator() {}

  void Configure();
  void Run();

private:
  void listenTcp();
  void listenUdp();
  void ReceiveFromLibrary(unsigned short address, unsigned char* data, int length);
  bool DiagnosticMessageReceived(unsigned short targetAddress);
  void CloseConnection();

private:
  std::unique_ptr<DoIPServer> server_;
  std::vector<std::thread> doip_receiver_{};
  std::unique_ptr<DoIPConnection> connection_{nullptr};
  bool server_active_{false};
};
