
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <map>

#include "tenth.pb.h"
#include "callback.h"



int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    {
        const std::string x_str = "accountid:9167,turnover:5470004.820000,rate:0.721400,fee:3484.170000,profit:-1398.990000,"
                                  "canceled:0.190000,failed:0.000000,acceptcost:49.391000,matchcost:1362.974000,base:3824359.000000,"
                                  "close_profit:-1398.990000,server:3,group:辉煌金星,";

        mam::AccountOverview ao;

        ao = structuralize_callback<mam::AccountOverview>(x_str);

        printf("AccountOverview::accountid=%d\n", ao.accountid());
        printf("AccountOverview::turnover=%f\n", ao.turnover());
        printf("AccountOverview::rate=%f\n", ao.rate());
        printf("AccountOverview::fee=%f\n", ao.fee());
        printf("AccountOverview::profit=%f\n", ao.profit());
        printf("AccountOverview::canceled=%f\n", ao.canceled());
        printf("AccountOverview::failed=%f\n", ao.failed());
        printf("AccountOverview::acceptcost=%f\n", ao.acceptcost());
        printf("AccountOverview::matchcost=%f\n", ao.matchcost());
        printf("AccountOverview::base=%f\n", ao.base());
        printf("AccountOverview::close_profit=%f\n", ao.close_profit());
        printf("AccountOverview::group=%s\n", ao.group().c_str());
        printf("\n");
    }

    {
        const std::string x_str = "accountid:8001,SYMBOL:000768.SZ,cfgvol:200,buyvol:200,sellvol:200,buyamount:6994.000000,"
                                  "sellamount:6976.000000,floatprofit:0.000000,settleprofit:-26.800000,fee:8.800000,server:11,";

        mam::ProfitDetail pd;

        pd = structuralize_callback<mam::ProfitDetail>(x_str);

        printf("ProfitDetail::accountid=%d\n", pd.accountid());
        printf("ProfitDetail::SYMBOL=%s\n", pd.symbol().c_str());
        printf("ProfitDetail::cfgvol=%d\n", pd.cfgvol());
        printf("ProfitDetail::buyvol=%d\n", pd.buyvol());
        printf("ProfitDetail::sellvol=%d\n", pd.sellvol());
        printf("ProfitDetail::buyamount=%f\n", pd.buyamount());
        printf("ProfitDetail::sellamount=%f\n", pd.sellamount());
        printf("ProfitDetail::floatprofit=%f\n", pd.floatprofit());
        printf("ProfitDetail::settleprofit=%f\n", pd.settleprofit());
        printf("ProfitDetail::fee=%f\n", pd.fee());
        printf("ProfitDetail::server=%d\n", pd.server());

        printf("\n");
    }

    {
        const std::string x_str = "id:8001,shcommission:10000/0.4,szcommission:10000/0.35,lowcost:5,as:Van,buyType:Dollar,sellType:Dollar,group:四五六,server:18,";

        mam::AccountInfo ai;

        ai = structuralize_callback<mam::AccountInfo>(x_str);

        printf("ProfitDetail::id=%d\n", ai.id());
        printf("ProfitDetail::shcommission=%s\n", ai.shcommission().c_str());
        printf("ProfitDetail::szcommission=%s\n", ai.szcommission().c_str());
        printf("ProfitDetail::lowcost=%d\n", ai.lowcost());
        printf("ProfitDetail::as=%s\n", ai.as().c_str());
        printf("ProfitDetail::buyType=%s\n", ai.buytype().c_str());
        printf("ProfitDetail::sellType=%s\n", ai.selltype().c_str());
        printf("ProfitDetail::group=%s\n", ai.group().c_str());
        printf("ProfitDetail::server=%s\n", ai.server().c_str());
    }

    return 0;
}
