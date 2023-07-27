
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include "twelfth.pb.h"
#include "pbcsv.hpp"

const std::string power_max_ao[] = {
    "accountid:124,turnover:6597932.00,rate:0.9796,fee:4200.65,profit:3387.53",
    "accountid:354,turnover:16234198.98,rate:0.9563,fee:10261.29,profit:-3666.94",
    "accountid:709,turnover:9662809.95,rate:0.8864,fee:6182.78,profit:4611.06",
};

const std::string power_max_di[] = {
    "KEY:huaxin-sz2,countSZ:0,countSH:222,countCF:0,countSZ_reverse:0,countSH_reverse:0,sumSZ_delay:0.000000,sumSH_delay:182.069619,sumCF_delay:0.000000,countSZ_delay:0,countSH_delay:0,countPdt_pos:0,countPdt_neg:0,countPdt_invalid:0,consume_CalFeat:15.543457,consume_CalPdt:47.300293,maxConsume_CalFeat:5.562012,maxConsume_CalPdt:0.246826,count_CalFeat:221,count_CalPdt:442,ntime:150130",
    "KEY:huaxin-sz2,countSZ:0,countSH:0,countCF:0,countSZ_reverse:0,countSH_reverse:0,sumSZ_delay:0.000000,sumSH_delay:0.000000,sumCF_delay:0.000000,countSZ_delay:0,countSH_delay:0,countPdt_pos:0,countPdt_neg:0,countPdt_invalid:0,consume_CalFeat:0.000000,consume_CalPdt:0.000000,maxConsume_CalFeat:0.000000,maxConsume_CalPdt:0.000000,count_CalFeat:0,count_CalPdt:0,ntime:150830",
    "KEY:huaxin-sz2,countSZ:0,countSH:0,countCF:0,countSZ_reverse:0,countSH_reverse:0,sumSZ_delay:0.000000,sumSH_delay:0.000000,sumCF_delay:0.000000,countSZ_delay:0,countSH_delay:0,countPdt_pos:0,countPdt_neg:0,countPdt_invalid:0,consume_CalFeat:0.000000,consume_CalPdt:0.000000,maxConsume_CalFeat:0.000000,maxConsume_CalPdt:0.000000,count_CalFeat:0,count_CalPdt:0,ntime:150930",
    "KEY:idc,countSZ:0,countSH:0,countCF:0,countSZ_reverse:0,countSH_reverse:0,sumSZ_delay:0.000000,sumSH_delay:0.000000,sumCF_delay:0.000000,countSZ_delay:0,countSH_delay:0,countPdt_pos:0,countPdt_neg:0,countPdt_invalid:0,consume_CalFeat:0.000000,consume_CalPdt:0.000000,maxConsume_CalFeat:0.000000,maxConsume_CalPdt:0.000000,count_CalFeat:0,count_CalPdt:0,ntime:91002",
    "KEY:idc,countSZ:2358,countSH:1804,countCF:0,countSZ_reverse:0,countSH_reverse:0,sumSZ_delay:1534.817671,sumSH_delay:2411.187561,sumCF_delay:0.000000,countSZ_delay:0,countSH_delay:532,countPdt_pos:0,countPdt_neg:0,countPdt_invalid:0,consume_CalFeat:0.000000,consume_CalPdt:0.000000,maxConsume_CalFeat:0.000000,maxConsume_CalPdt:0.000000,count_CalFeat:0,count_CalPdt:0,ntime:91102",
    "KEY:idc,countSZ:6362,countSH:7387,countCF:0,countSZ_reverse:0,countSH_reverse:107,sumSZ_delay:3933.055926,sumSH_delay:5338.154604,sumCF_delay:0.000000,countSZ_delay:0,countSH_delay:0,countPdt_pos:0,countPdt_neg:0,countPdt_invalid:0,consume_CalFeat:0.000000,consume_CalPdt:0.000000,maxConsume_CalFeat:0.000000,maxConsume_CalPdt:0.000000,count_CalFeat:0,count_CalPdt:0,ntime:91702",
};

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    mam::LoadResult lr;

    const std::string type_name = lr.GetDescriptor()->name();
    const int field_count = lr.GetDescriptor()->field_count();
    const google::protobuf::Reflection * ref = lr.GetReflection();

    std::vector<std::string> keys { "Hello", "World" };

    for (int i = 0; i < field_count; i++) {
        const google::protobuf::FieldDescriptor * f = lr.GetDescriptor()->field(i);

        printf("f->type_name=%s,f->name=%s\n", f->type_name(), f->name().c_str());

        for (const auto & mk : keys) {
            switch (f->type()) {
                case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
                {
                    assert(f->is_map() && f->is_repeated());

                    google::protobuf::Message * _map_msg = ref->AddMessage(&lr, f);
                    const google::protobuf::Descriptor * _map_msg_des = _map_msg->GetDescriptor();
                    const google::protobuf::Reflection * _map_msg_ref = _map_msg->GetReflection();
                    assert(_map_msg_des->field_count() == 2);
                    // key
                    const google::protobuf::FieldDescriptor * _map_msg_key_f = _map_msg_des->FindFieldByName("key");
                    _map_msg_ref->SetString(_map_msg, _map_msg_key_f, mk);
                    // value
                    const google::protobuf::FieldDescriptor * _map_msg_val_f = _map_msg_des->FindFieldByName("value");
                    google::protobuf::Message * _map_msg_val = _map_msg_ref->MutableMessage(_map_msg, _map_msg_val_f);
                    const google::protobuf::Descriptor * _map_msg_val_des = _map_msg_val->GetDescriptor();
                    const google::protobuf::Reflection * _map_msg_val_ref = _map_msg_val->GetReflection();
                    
                    const int _map_msg_val_field_count = _map_msg_val_des->field_count();
                    for (int i = 0; i < _map_msg_val_field_count; i++) {
                        const google::protobuf::FieldDescriptor * _map_msg_val_member_f = _map_msg_val_des->field(i);

                        std::map<std::string, google::protobuf::Message*> _map_key_array;

                        if (_map_msg_val_member_f->is_map() && _map_msg_val_member_f->is_repeated()) {
                            
                            for (size_t k = 0; k < sizeof(power_max_di) / sizeof(power_max_di[0]); k++) {
                                std::string line = power_max_di[k];
                                size_t pos = line.find_first_of(',');

                                std::string key_in_line = std::string(line.begin(), line.begin() + pos);
                                key_in_line = std::string(key_in_line.begin() + 4, key_in_line.end());      // strlen("KEY:") = 4

                                if (_map_key_array.find(key_in_line) == _map_key_array.end()) {
                                    google::protobuf::Message * _map_msg_val_member = _map_msg_val_ref->AddMessage(_map_msg_val, _map_msg_val_member_f);
                                    const google::protobuf::Descriptor * _map_msg_member_des = _map_msg_val_member->GetDescriptor();
                                    const google::protobuf::Reflection * _map_msg_member_ref = _map_msg_val_member->GetReflection();
                                    assert(_map_msg_member_des->field_count() == 2);
                                    // little key
                                    const google::protobuf::FieldDescriptor * _map_msg_val_member_key_f = _map_msg_member_des->FindFieldByName("key");
                                    _map_msg_member_ref->SetString(_map_msg_val_member, _map_msg_val_member_key_f, key_in_line);
                                    // little value
                                    const google::protobuf::FieldDescriptor * _map_msg_val_member_val_f = _map_msg_member_des->FindFieldByName("value");
                                    google::protobuf::Message * _map_msg_val_member_val_array = _map_msg_member_ref->MutableMessage(_map_msg_val_member, _map_msg_val_member_val_f);
                                    _map_key_array.insert(std::make_pair(key_in_line, _map_msg_val_member_val_array));
                                }
                                
                                const google::protobuf::Descriptor * _map_msg_val_member_val_array_des = _map_key_array[key_in_line]->GetDescriptor();
                                const google::protobuf::Reflection * _map_msg_val_member_val_array_ref = _map_key_array[key_in_line]->GetReflection();
                                
                                const google::protobuf::FieldDescriptor * _map_msg_val_member_val_array_f = _map_msg_val_member_val_array_des->field(0);

                                std::string val_in_line = std::string(line.begin() + pos + 1, line.end());

                                google::protobuf::Message * _map_msg_array_msg_ele_msg = _map_msg_val_member_val_array_ref->AddMessage(_map_key_array[key_in_line],
                                                                                                                                        _map_msg_val_member_val_array_f);
                                bool bl = csv2pb(_map_msg_array_msg_ele_msg, val_in_line);
                                if (! bl) {
                                    _map_msg_val_member_val_array_ref->RemoveLast(_map_key_array[key_in_line], _map_msg_val_member_val_array_f);
                                }
                            }
                        } else if (! _map_msg_val_member_f->is_map() && _map_msg_val_member_f->is_repeated()) {
                            for (size_t k = 0; k < sizeof(power_max_ao) / sizeof(power_max_ao[0]); k++) {
                                std::string line = power_max_ao[k];
                                google::protobuf::Message * _map_msg_array_msg_ele_msg = _map_msg_val_ref->AddMessage(_map_msg_val, _map_msg_val_member_f);
                                bool bl = csv2pb(_map_msg_array_msg_ele_msg, line);
                                if (! bl) {
                                    _map_msg_val_ref->RemoveLast(_map_msg_val, _map_msg_val_member_f);
                                }
                            }
                        } else {
                            // printf("field: %s is not map or array", _map_msg_val_member_f->full_name().c_str());    // too much log, comment it out
                            continue;
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
    }      

    // print to console
    {
        printf("\n");
        decltype(lr.map_account_overview().begin()) it;
        for (it = lr.map_account_overview().begin(); it != lr.map_account_overview().end(); it++) {
            printf("key=%s,size=%d\n", it->first.c_str(),  it->second.content_size());
            
            for (int i = 0; i < it->second.content_size(); i++) {
                const mam::AccountOverview & x = it->second.content(i);
                printf("accountid=%d,turnover=%.2f,rate=%.4f,fee=%.2f,profit=%.2f\n", x.accountid(), x.turnover(), x.rate(), x.fee(), x.profit());
            }
        }
    }

    {
        printf("\n");
        decltype(lr.map_data_index_list().begin()) it;
        for (it = lr.map_data_index_list().begin(); it != lr.map_data_index_list().end(); it++) {
            printf("key=%s, size=%d\n", it->first.c_str(), it->second.dic_size());
            decltype(it->second.dic().begin()) subit;
            for (subit = it->second.dic().begin(); subit != it->second.dic().end(); subit++) {
                printf("  subkey=%s, size=%d\n", subit->first.c_str(), subit->second.snapshots_size());
                for (int i = 0; i < subit->second.snapshots_size(); i++) {
                    const mam::SysInfo & x = subit->second.snapshots(i);
                    printf("countSZ=%d,countSH=%d,countCF=%d,countSZ_reverse=%d,countSH_reverse=%d,sumSZ_delay=%f,sumSH_delay=%f\n",
                            x.countsz(), x.countsh(), x.countcf(), x.countsz_reverse(), x.countsh_reverse(), x.sumsh_delay(), x.sumsh_delay());
                }
            }
        }
    }

    return 0;
}
