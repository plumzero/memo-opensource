
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <stdexcept>

#include "eleventh.pb.h"
#include "pbcsv.hpp"


const std::string power_max_ao[] = {
    "accountid:124,turnover:6597932.00,rate:0.9796,fee:4200.65,profit:3387.53",
    "accountid:354,turnover:16234198.98,rate:0.9563,fee:10261.29,profit:-3666.94",
    "accountid:709,turnover:9662809.95,rate:0.8864,fee:6182.78,profit:4611.06",
};

const std::string power_max_pd[] = {
    "accountid:124,symbol:002131.SZ,cfgvol:50800,buyvol:31800,sellvol:31800",
    "accountid:679,symbol:603986.SH,cfgvol:13500,buyvol:13500,sellvol:13406",
};

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    mam::SaveRequest request;

    const std::string type_name = request.GetDescriptor()->name();
    const int field_count = request.GetDescriptor()->field_count();
    const google::protobuf::Reflection * ref = request.GetReflection();

    for (int i = 0; i < field_count; i++) {
        const google::protobuf::FieldDescriptor * f = request.GetDescriptor()->field(i);
        printf("f->type_name=%s,f->name=%s,f->full_name=%s, f->json_name=%s,f->xname=%s\n",
                f->type_name(), f->name().c_str(), f->full_name().c_str(), f->json_name().c_str(), f->camelcase_name().c_str());
        switch (f->type()) {
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                if (f->is_repeated()) {
                    for (int j = 0; j < 5; j++) {   // 向 repeated 中添加 5 个元素
                        ref->AddString(&request, f, "Hello_" + std::to_string(j));
                    }
                } else if (f->is_optional()) {
                    ref->SetString(&request, f, "2021-02-02");
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
            {
                printf("is a message\n ");
                if (f->camelcase_name() == "accountOverview") {
                    for (size_t j = 0; j < sizeof(power_max_ao) / sizeof(power_max_ao[0]); j++) {
                        google::protobuf::Message * submsg =  ref->AddMessage(&request, f);
                        csv2pb(submsg, power_max_ao[j]);
                    }
                } else if (f->camelcase_name() == "profitDetail") {
                    for (size_t j = 0; j < sizeof(power_max_pd) / sizeof(power_max_pd[0]); j++) {
                        google::protobuf::Message * submsg =  ref->AddMessage(&request, f);
                        csv2pb(submsg, power_max_pd[j]);
                    }
                }
            }
            break;
            default:
            {
                ;
            }
        }
    }

    printf("\ndate=%s\n", request.date().c_str());
    
    for (int i = 0; i < request.strs_size(); i++) {
        const std::string & x = request.strs(i);
        printf("%s ", x.c_str());
    }
    printf("\n");

    for (int i = 0; i < request.account_overview_size(); i++) {
        const mam::AccountOverview & x = request.account_overview(i);
        printf("accountid=%d,turnover=%.2f,rate=%.4f,fee=%.2f,profit=%.2f\n", x.accountid(), x.turnover(), x.rate(), x.fee(), x.profit());
    }

    for (int i = 0; i < request.profit_detail_size(); i++) {
        const mam::ProfitDetail & x = request.profit_detail(i);
        printf("accountid=%d,symbol=%s,cfgvol=%d,buyvol=%d,sellvol=%d\n", x.accountid(), x.symbol().c_str(), x.cfgvol(), x.buyvol(), x.sellvol());
    }

    return 0;
}
