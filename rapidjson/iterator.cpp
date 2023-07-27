
#include "rapidjson/document.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

const std::string& json_text = R"JSON(

{
    "LogLevel" : "DBUG",
    "LogDir"   : "./",
    "LogSize"  : 5,
    
    "Daemon" : {
        "type"  : "transfer",
        "as"    : "daemon",
        "on"    : "yes",
        "listen" : [
            {
                "host" : "127.0.0.1",
                "port" : 12345
            }
        ]
    },
    
    "Rat" : {
        "type" : "proxy",
        "as"   : "child",
        "on"   : "yes",
        "listen" : [
            {
                "host" : "127.0.0.1",
                "port" : 10031
            }
        ],
        "connect" : [
            {
                "host" : "127.0.0.1",
                "port" : 12345
            }
        ]
    },
    
    "Ox" : {
        "type" : "proxy",
        "as"   : "child",
        "on"   : "yes",
        "listen" : [
            {
                "host" : "127.0.0.1",
                "port" : 10032
            }
        ],
        "connect" : [
            {
                "host" : "127.0.0.1",
                "port" : 12345
            }
        ]
    },
    
    "DreamCollapse" : {
        "type" : "business",
        "as"   : "child",
        "on"   : "yes",
        "connect" : [
            {
                "host" : "127.0.0.1",
                "port" : 12345
            }
        ]
    },
    
    "Altergeist" : {
        "type" : "business",
        "as"   : "child",
        "on"   : "yes",
        "connect" : [
            {
                "host" : "127.0.0.1",
                "port" : 12345
            }
        ]
    }
}

)JSON";

void parse(const rapidjson::Value& val, int& offset)
{
    assert(val.IsObject());
    
    offset++;
    decltype (val.MemberBegin()) itr;
    for (itr = val.MemberBegin(); itr != val.MemberEnd(); itr++) {  
        std::cout << std::setw(offset * 4) << itr->name.GetString() << " : ";
        if (itr->value.IsInt()) {
            std::cout << itr->value.GetInt() << std::endl;
        } else if (itr->value.IsString()) { 
            std::cout << itr->value.GetString() << std::endl; 
        } else if (itr->value.IsArray()) {
            std::cout << std::endl;
            for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++) {
                if (itr->value[i].IsInt()) {
                    std::cout << itr->value[i].GetInt();
                } else if (itr->value[i].IsString()) {
                    std::cout << itr->value[i].GetString();
                } else if (itr->value[i].IsObject()) {
                    parse(itr->value[i], offset);
                }
            }
        } else if (itr->value.IsObject()) {
            std::cout << std::endl;
            parse(itr->value, offset);
        }
    }
    offset--;
}

int main()
{
    rapidjson::Document doc;

    doc.Parse(json_text.c_str());
    
    int offset = 0;
    parse(doc, offset);
    
    return 0;
}