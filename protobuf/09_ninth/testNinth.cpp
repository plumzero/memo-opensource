
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <exception>

#include "ninth.pb.h"

#include "callback.h"

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    mam::CardSet card;
    
    // fill with value
    card.set_omit("omit this field");
    card.mutable_ca1()->set_altergeist("Meluseek");
    card.mutable_cf1()->set_performage("Plushfire");
    card.mutable_cg1()->set_galatea("the Orcust Automaton");
    card.mutable_cg2()->set_galatea("the Orcust Harp Horror");
    card.mutable_cm1()->set_madolche("Queen Tiaramisu");
    card.mutable_cm2()->set_madolche("Chateau");
    card.mutable_cm3()->set_madolche("Waltz");
    card.mutable_cz1()->set_zoodiac("Drident");

    const google::protobuf::Reflection* ref = card.GetReflection();

    const int fc = card.GetDescriptor()->field_count();

    for (int i = 0; i < fc; i++) {
        const google::protobuf::FieldDescriptor * f = card.GetDescriptor()->field(i);

        // 变量名过滤        
        if (f->name() == "omit") {
            printf("omit it\n");
            continue;
        }

        std::string o_type = f->options().GetExtension(mam::opt).type();
        int32_t o_max = f->options().GetExtension(mam::opt).max();

        // 可选项字段过滤 - type 过滤
        if (o_type == "Trap") {
            printf("filter by type: %s\n", f->name().c_str());
            continue;
        }

        // 可选项字段过滤 - max 过滤
        if (o_max == 1) {
            printf("filter by max: %s\n", f->name().c_str());
            continue;
        }

        const google::protobuf::Message & info = ref->GetMessage(card, f);
        serialize_callback(info);        
    }

    return 0;
}
