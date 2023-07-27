
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <stdexcept>

#include "eighth.pb.h"
#include "callback.h"

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    mam::AccountOverviews aos;
    mam::AccountOverview * ao;

    aos.set_id(123456);

    // the first group
    {
        ao = aos.mutable_content()->Add();
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
    // the second group
    {
        ao = aos.mutable_content()->Add();
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
    // the third group
    {
        ao = aos.mutable_content()->Add();
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

    serialize_callback(aos);

    return 0;
}
