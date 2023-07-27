
#include <cstdio>

#include "seventh.pb.h"

int main()
{
    {
        mam::IntArray ia;

        ia.mutable_iarray()->Add(234);
        ia.mutable_iarray()->Add(345);
        ia.mutable_iarray()->Add(456);
        ia.mutable_iarray()->Add(567);
        ia.mutable_iarray()->Add(678);

        int i;
        for (i = 0; i < ia.iarray_size(); i++) {
            printf("%d ", ia.iarray(i));
        }
        printf("\n");
        
        // 反射测试
        const google::protobuf::Descriptor * desc = ia.GetDescriptor();
        const google::protobuf::Reflection * ref = ia.GetReflection();

        int cnt = desc->field_count();  // 1

        for (i = 0; i < cnt; i++) {
            const google::protobuf::FieldDescriptor * f = ia.GetDescriptor()->field(i); // f->type == TYPE_INT32

            printf("f->type=%d, f->type_name=%s\n", f->type(), f->type_name());

            int size = ref->FieldSize(ia, f);
            for (int j = 0; j < size; j++) {
                int val = ref->GetRepeatedInt32(ia, f, j);
                printf("%d ", val);
            }
            printf("\n");
        }
    }

    printf("-------------------------\n");

    {
        mam::StArray sa;
        mam::SubType * st;

        st = sa.mutable_sts()->Add();
        st->set_y_i32(321);

        st = sa.mutable_sts()->Add();
        st->set_y_i32(654);

        st = sa.mutable_sts()->Add();
        st->set_y_i32(987);

        int i;
        for (i = 0; i < sa.sts_size(); i++) {
            const mam::SubType & ref_st = sa.sts(i);
            printf("%d ", ref_st.y_i32());
        }
        printf("\n");

        // 反射测试
        const google::protobuf::Descriptor * desc = sa.GetDescriptor();
        const google::protobuf::Reflection * ref = sa.GetReflection();

        int cnt = desc->field_count();

        printf("cnt=%d\n", cnt);

        for (i = 0; i < cnt; i++) {
            const google::protobuf::FieldDescriptor * f = sa.GetDescriptor()->field(i); // f->type == TYPE_MESSAGE

            printf("f->type=%d, f->type_name=%s\n", f->type(), f->type_name());

            int size = ref->FieldSize(sa, f);

            printf("size=%d\n", size);
            for (int j = 0; j < size; j++) {
                const google::protobuf::Message & submsg = ref->GetRepeatedMessage(sa, f, j);
                const std::string tn = submsg.GetDescriptor()->name();
                printf("xtn=%s,field_count=%d: ", tn.c_str(), submsg.GetDescriptor()->field_count());
                const google::protobuf::Reflection* subref = submsg.GetReflection();
                
                for (int k = 0; k < submsg.GetDescriptor()->field_count(); k++) {
                    const google::protobuf::FieldDescriptor * subf = submsg.GetDescriptor()->field(k);
                    const int32_t sub_i32 = subref->GetInt32(submsg, subf);
                    printf("%d ", sub_i32);
                }
                printf("\n");
            }
            printf("\n");
        }
    }

    return 0;
}