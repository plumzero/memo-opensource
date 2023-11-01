
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>

int main()
{
  std::ifstream ifs("array.json", std::fstream::in | std::fstream::binary);
  if (!ifs.is_open()) {
    return -1;
  }

  nlohmann::json j;
  ifs >> j;
  ifs.close();

  for (size_t i = 0; i < j.size(); i++) {
    std::cout << j[i] << std::endl;
    std::string name = j[i]["name"].get<std::string>();
    bool on = j[i]["on"].get<bool>();
    bool burn = j[i]["burn"].get<bool>();
    std::string connect = j[i]["connect"].get<std::string>();
    printf("name=%s,on=%d,burn=%d,connect=%s\n", name.c_str(), on, burn, connect.c_str());
  }

  return 0;
}