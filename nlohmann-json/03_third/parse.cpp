
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stdio.h>

// str: [In] like "0x1234" or "0X1234"
int hextodec(const std::string& str)
{
  std::stringstream ss;
  ss << std::hex << str;
  int val;
  ss >> val;

  return val;
}

int main()
{
  std::ifstream ifs("my.json", std::fstream::in | std::fstream::binary);
  if (!ifs.is_open()) {
    return -1;
  }

  nlohmann::json j;
  ifs >> j;
  ifs.close();

  {
    std::string host = j["host"].get<std::string>();
    std::cout << host << std::endl;
  }

  {
    int port = j["port"].get<int>();
    std::cout << port << std::endl;
  }

  {
    std::string type = j["type"].get<std::string>();
    std::cout << type << std::endl;
  }

  {
    std::string logical = j["logical"].get<std::string>();
    std::cout << logical << std::endl;
    int i = hextodec(logical);
    std::cout << std::hex << i << std::endl;
  }

  {
    std::array<std::string, 6> sarr = j["eid"].get<std::array<std::string, 6>>();
    std::array<uint8_t, 6> iarr;
    for (size_t i = 0; i < 6; i++) {
      iarr[i] = hextodec(sarr[i]);
    }

    for (auto & elem : iarr) {
      printf("%02X ", (int)elem);
    }
    printf("\n");
  }

  {
    std::array<std::string, 6> sarr = j["gid"].get<std::array<std::string, 6>>();
    std::array<uint8_t, 6> iarr;
    for (size_t i = 0; i < 6; i++) {
      iarr[i] = hextodec(sarr[i]);
    }

    for (auto & elem : iarr) {
      printf("%02X ", (int)elem);
    }
    printf("\n");
  }

  {
    std::cout << std::boolalpha << j["broadcast"].is_null() << std::endl;
    if (!j["broadcast"].is_null()) {
      std::vector<int> vec = j["broadcast"].get<std::vector<int>>();
      for (size_t i = 0; i < vec.size(); i++) {
        printf("%d ", vec[i]);
      }
      printf("\n");
    }
  }

  return 0;
}