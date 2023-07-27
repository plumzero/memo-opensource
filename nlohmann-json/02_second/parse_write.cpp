
#include "trjson.h"
#include <stdio.h>

class Period {
public:
    std::string name{"001122"};
    std::string store_path{"/path/to/store"};
    std::string bag_name{"AlgData.bag"};
    int32_t collect_time{5};
public:
    void print() {
        printf("name=%s,store_path=%s,bag_name=%s,collect_time=%d\n", name.c_str(), store_path.c_str(), bag_name.c_str(), collect_time);
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Period,
                                   name,
                                   store_path,
                                   bag_name,
                                   collect_time);

int main()
{
    Period period;
    TypeJson<Period> typejson(period, "hello.json");
    
    // typejson.write();
    // period.print();

    typejson.parse();
    period.print();
    
    period.name = "334455";
    period.store_path = "/tmp";
    period.bag_name = "my.bag";
    period.collect_time = 0;
    typejson.write();
    period.print();

    return 0;
}