
#include "tortellini.hh"
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <array>
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

std::array<uint8_t, 6> hexstoarray(const std::string& str)
{
  std::stringstream ss;
  ss << std::hex << str;

  std::vector<int> values;
  std::copy(std::istream_iterator<int>(ss), std::istream_iterator<int>(), std::back_inserter(values));
  
  values.resize(6);

  std::array<uint8_t, 6> arr;
  for (size_t i = 0; i < values.size(); i++) {
    arr[i] = values[i];
  }

  return arr;
}

int main()
{
  tortellini::ini ini;

  std::string conf = "my.ini";

  std::ifstream ifs(conf.c_str(), std::ifstream::in | std::ifstream::binary);
  if (! ifs.is_open()) {
    return -1;
  }
  ifs >> ini;
  ifs.close(); 

  std::string section = "Gateway";

  {
    std::string key = "vin";
    std::string val = ini[section][key] | "";
    std::cout << section << " -> " << key << " -> " << val << std::endl;
  }

  {
    std::string key = "type";
    std::string val = ini[section][key] | "";
    std::cout << section << " -> " << key << " -> " << val << std::endl;
  }

  {
    std::string key = "logical";
    std::string val = ini[section][key] | "";
    std::cout << section << " -> " << key << " -> " << val << std::endl;
    printf("hextodec(%s)=%04X\n", val.c_str(), hextodec(val));
  }

  {
    std::string key = "eid";
    std::string val = ini[section][key] | "";
    std::cout << section << " -> " << key << " -> " << val << std::endl;

    auto values = hexstoarray(val);
    std::copy(values.begin(), values.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
  }

  {
    std::string key = "gid";
    std::string val = ini[section][key] | "";
    std::cout << section << " -> " << key << " -> " << val << std::endl;

    auto values = hexstoarray(val);
    std::copy(values.begin(), values.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
  }

  return 0;
}