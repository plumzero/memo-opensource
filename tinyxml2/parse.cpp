
#include "tinyxml2.h"
#include <stdio.h>
#include <iostream>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("%s <xml-file>\n", argv[0]);
    return -1;
  }

  tinyxml2::XMLDocument doc;
	doc.LoadFile(argv[1]);

  {
    // Configure.EcuVersion
    bool container_bool;
    tinyxml2::XMLElement* element = doc.FirstChildElement("Configure")->FirstChildElement("EcuVersion");
    element->QueryBoolAttribute("Used", &container_bool);
    const char* val = element->GetText();
    std::cout << std::boolalpha;
    std::cout << "EcuVersion.Used=" << container_bool << ", EcuVersion.val=" << val << std::endl;
  }

  {
    // Configure.EcuSoftwareCode
    bool container_bool;
    tinyxml2::XMLElement* element = doc.FirstChildElement("Configure")->FirstChildElement("EcuSoftwareCode");
    element->QueryBoolAttribute("Used", &container_bool);
    const char* val = element->GetText();
    std::cout << std::boolalpha;
    std::cout << "EcuSoftwareCode.Used=" << container_bool << ", EcuSoftwareCode.val=" << val << std::endl;
  }

  {
    // Configure.DoIP_SA
    bool container_bool;
    int conatiner_int;
    tinyxml2::XMLElement* element = doc.FirstChildElement("Configure")->FirstChildElement("DoIP_SA");
    element->QueryBoolAttribute("Used", &container_bool);
    element->QueryIntText(&conatiner_int);
    std::cout << std::boolalpha;
    std::cout << "DoIP_SA.Used=" << container_bool << ", DoIP_SA.val=" << conatiner_int << std::endl;
  }

  {
    // Configure.FilePath
    bool container_bool;
    tinyxml2::XMLElement* element = doc.FirstChildElement("Configure")->FirstChildElement("FilePath");
    element->QueryBoolAttribute("Used", &container_bool);
    const char* val = element->GetText();
    std::cout << std::boolalpha;
    std::cout << "FilePath.Used=" << container_bool << ", FilePath.val=" << val << std::endl;
  }

  {
    // Configure.MD5
    bool container_bool;
    const char* container_string = nullptr;
    tinyxml2::XMLElement* element = doc.FirstChildElement("Configure")->FirstChildElement("MD5");
    element->QueryBoolAttribute("Used", &container_bool);
    element->QueryStringAttribute("FileName", &container_string);
    const char* val = element->GetText();
    std::cout << std::boolalpha;
    std::cout << "MD5.Used=" << container_bool << ", MD5.FileName=" << container_string << ", MD5.val=" << val << std::endl; 
  }

  return 0;
}
