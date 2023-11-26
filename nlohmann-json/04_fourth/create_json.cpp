
#include "json.hpp"
#include <string>
#include <stdio.h>

int main()
{
  {
    // {"keyTime":1647324409131,"keyList":"xxxxxxxxxxxxxxxxxxxxxxx"}
    nlohmann::json j;
    j["keyTime"] = 1647324409131;
    j["keyList"] = "xxxxxxxxxxxxxxxxxxxxxxx";

    std::string stream = j.dump(4);
    printf("%s\n", stream.c_str());
  }

  {
    // [{"Key_128":"466132366936486e416e436a61424a61"},{"Key_256":"636c3765363234557a6b34716a4d6154"},{"Key_SecOC":"4c31515751596c4f536d316c3544717a"}]
    nlohmann::json j;
    int i = 0;
    {
      std::string key = "Key_128";
      std::string val = "466132366936486e416e436a61424a61";
      j[i++] = {{ key, val }};
    }
    {
      std::string key = "Key_256";
      std::string val = "636c3765363234557a6b34716a4d6154";
      j[i++] = {{ key, val }};
    }
    {
      std::string key = "Key_SecOC";
      std::string val = "4c31515751596c4f536d316c3544717a";
      j[i++] = {{ key, val }};
    }

    std::string stream = j.dump(2);
    printf("%s\n", stream.c_str());

    nlohmann::json k = nlohmann::json::parse(stream.c_str());
    printf("%s\n", k.dump().c_str());

    nlohmann::json l;
    l["keyTime"] = 1647324409131;
    l["keyList"] = k;
    printf("%s\n", l.dump(2).c_str());
  }

  {
    printf("\n");
    nlohmann::json j;

    try {
      j["keyTime"].get<int64_t>();
    } catch(...) {
      printf("no this\n");
    }
  }

  return 0;
}