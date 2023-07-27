
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <stdexcept>

#include "tenth.pb.h"
#include "callback.h"

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    mam::SaveRequest request;

    request.set_mode(mam::SaveMode::Create);
    request.set_date("2021-01-25");

    mam::AccountOverview * ao;
    // AccountOverview: the first group
    {
        ao = request.mutable_account_overview()->Add();
        ao->set_accountid(341);
        ao->set_turnover(6597932.00);
        ao->set_rate(0.9796);
        ao->set_fee(4200.65);
        ao->set_profit(3387.53);
        ao->set_canceled(0.01);
        ao->set_failed(0.02);
        ao->set_acceptcost(9.958);
        ao->set_matchcost(524.061);
        ao->set_base(3420274.00);
        ao->set_close_profit(3095.61);
        ao->set_server(11);
        ao->set_group("魔机马达");
    }
    // AccountOverview: the second group
    {
        ao = request.mutable_account_overview()->Add();
        ao->set_accountid(343);
        ao->set_turnover(16234198.98);
        ao->set_rate(0.9563);
        ao->set_fee(10261.29);
        ao->set_profit(-3666.94);
        ao->set_canceled(0.13);
        ao->set_failed(0.01);
        ao->set_acceptcost(11.054);
        ao->set_matchcost(823.008);
        ao->set_base(8630673.00);
        ao->set_close_profit(-3645.85);
        ao->set_server(11);
        ao->set_group("黄泉天轮");
    }
    // AccountOverview: the third group
    {
        ao = request.mutable_account_overview()->Add();
        ao->set_accountid(9167);
        ao->set_turnover(5470004.82);
        ao->set_rate(0.7214);
        ao->set_fee(3484.17);
        ao->set_profit(-1398.99);
        ao->set_canceled(0.19);
        ao->set_failed(0.00);
        ao->set_acceptcost(49.391);
        ao->set_matchcost(1362.974);
        ao->set_base(3824359.00);
        ao->set_close_profit(-1398.99);
        ao->set_server(3);
        ao->set_group("辉煌金星");
    }

    mam::ProfitDetail * pd;
    // ProfitDetail: the first group
    {
        pd = request.mutable_profit_detail()->Add();
        pd->set_accountid(8001);
        pd->set_symbol("000768.SH");
        pd->set_cfgvol(200);
        pd->set_buyvol(200);
        pd->set_sellvol(200);
        pd->set_buyamount(6994.00);
        pd->set_sellamount(6976.00);
        pd->set_floatprofit(0.00);
        pd->set_settleprofit(-26.80);
        pd->set_fee(8.80);
        pd->set_server(11);
    }
    // ProfitDetail: the second group
    {
        pd = request.mutable_profit_detail()->Add();
        pd->set_accountid(8002);
        pd->set_symbol("600219.SZ");
        pd->set_cfgvol(3500);
        pd->set_buyvol(3500);
        pd->set_sellvol(3500);
        pd->set_buyamount(12599.00);
        pd->set_sellamount(12635.00);
        pd->set_floatprofit(-0.00);
        pd->set_settleprofit(19.55);
        pd->set_fee(16.45);
        pd->set_server(11);
    }

    mam::AccountInfo * ai;
    // AccountInfo: the first group
    {
        ai = request.mutable_account_info()->Add();
        ai->set_id(9305);
        ai->set_shcommission("10000/5");
        ai->set_szcommission("10000/3");
        ai->set_lowcost(5);
        ai->set_as("Gandora-X");
        ai->set_buytype("RMB");
        ai->set_selltype("RMB");
        ai->set_group("一二三");
        ai->set_server("15");
    }

    // AccountInfo: the second group
    {
        ai = request.mutable_account_info()->Add();
        ai->set_id(9306);
        ai->set_shcommission("10000/0.4");
        ai->set_szcommission("10000/0.35");
        ai->set_lowcost(5);
        ai->set_as("Van");
        ai->set_buytype("Dollar");
        ai->set_selltype("Dollar");
        ai->set_group("四五六");
        ai->set_server("18");
    }

    const std::string type_name = request.GetDescriptor()->name();
    const int field_count = request.GetDescriptor()->field_count();
    const google::protobuf::Reflection * ref = request.GetReflection();

    for (int i = 0; i < field_count; i++)
    {
        const google::protobuf::FieldDescriptor * field_descriptor = request.GetDescriptor()->field(i);
        switch (field_descriptor->type())
        {
            case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
            {
                if (field_descriptor->is_map() && field_descriptor->is_repeated()) {
                    // not here
                } else if (field_descriptor->is_repeated()) {
                    int repeated_count = ref->FieldSize(request, field_descriptor);
                    for (int j = 0; j < repeated_count; j++) {
                        const google::protobuf::Message & realmsg = ref->GetRepeatedMessage(request, field_descriptor, j);
                        bool bl = check_callback(realmsg);
                        if (! bl) {
                            printf("=========== check failed =============\n");
                        } else {
                            serialize_callback(realmsg);
                        }
                    }
                } else if (field_descriptor->is_optional()) {
                    // not here
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
            case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
            case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
            case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
            case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
            case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
            case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                // not here
            }
            break;
            default:
            {
                continue;
            }
        }
    }

    return 0;
}
