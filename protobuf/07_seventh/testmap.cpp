
#include <cstdio>

#include "seventh.pb.h"

int main()
{
    {
        mam::IntMap im;

        im.mutable_intstring()->insert(google::protobuf::MapPair<int32_t, std::string>(100, "C++"));
        im.mutable_intstring()->insert(google::protobuf::MapPair<int32_t, std::string>(200, "Perl"));
        im.mutable_intstring()->insert(google::protobuf::MapPair<int32_t, std::string>(300, "Shell"));

        decltype(im.intstring().begin()) it;
        for (it = im.intstring().begin(); it != im.intstring().end(); it++) {
            printf("key=%d, value=%s\n", it->first, it->second.c_str());
        }

        // 反射测试
        const google::protobuf::Descriptor * desc = im.GetDescriptor();
        const google::protobuf::Reflection * ref = im.GetReflection();

        int cnt = desc->field_count();
        printf("cnt=%d\n", cnt);

        for (int i = 0; i < cnt; i++) {
            const google::protobuf::FieldDescriptor * f = im.GetDescriptor()->field(i);
            printf("f->type=%d, f->type_name=%s\n", f->type(), f->type_name());
            int _map_size = ref->FieldSize(im, f);
            printf("_map_size=%d\n", _map_size);
            for (int j = 0; j < _map_size; j++) {
                for (int k = 0; k < 2; k++) {
                    const google::protobuf::Message & _im = ref->GetRepeatedMessage(im, f, j);
                    const google::protobuf::FieldDescriptor * _f = _im.GetDescriptor()->field(k);
                    switch (_f->type()) {
                        case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
                        {
                            int32_t key = _im.GetReflection()->GetInt32(_im, _f);
                            printf("key=%d,", key);
                        }
                        break;
                        case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
                        {
                            std::string str = _im.GetReflection()->GetString(_im, _f);
                            printf("val=%s\n", str.c_str());
                        }
                        break;
                        default:
                        {
                            ;   // only adapt to test
                        }
                    }
                }
            }
        }
    }

    printf("---------------------------------------------------\n");

    {
        mam::IntObjMap iom;
        mam::SubType st;

        st.set_y_i32(123);
        iom.mutable_intobjmap()->insert(google::protobuf::MapPair<std::string, mam::SubType>("C++", st));

        st.set_y_i32(456);
        iom.mutable_intobjmap()->insert(google::protobuf::MapPair<std::string, mam::SubType>("Perl", st));
        
        st.set_y_i32(789);
        iom.mutable_intobjmap()->insert(google::protobuf::MapPair<std::string, mam::SubType>("Lua", st));

        decltype(iom.intobjmap().begin()) it;
        for (it = iom.intobjmap().begin(); it != iom.intobjmap().end(); it++) {
            printf("key=%s,value=%d\n", it->first.c_str(), it->second.y_i32());
        }

        // 反射测试
        const google::protobuf::Descriptor * desc = iom.GetDescriptor();
        const google::protobuf::Reflection * ref = iom.GetReflection();        

        int cnt = desc->field_count();
        printf("cnt=%d\n", cnt);

        for (int i = 0; i < cnt; i++) {
            const google::protobuf::FieldDescriptor * f = iom.GetDescriptor()->field(i);
            printf("f->type=%d, f->type_name=%s\n", f->type(), f->type_name());
            int _map_size = ref->FieldSize(iom, f);
            printf("_map_size=%d\n", _map_size);
            for (int j = 0; j < _map_size; j++) {
                for (int k = 0; k < 2; k++) {
                    const google::protobuf::Message & _iom = ref->GetRepeatedMessage(iom, f, j);
                    const google::protobuf::FieldDescriptor * _f = _iom.GetDescriptor()->field(k);
                    switch (_f->type()) {
                        case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
                        {
                            std::string str = _iom.GetReflection()->GetString(_iom, _f);
                            printf("key=%s,", str.c_str());
                        }
                        break;
                        case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
                        {
                            const google::protobuf::Message & __m = _iom.GetReflection()->GetMessage(_iom, _f);
                            const int __fc = __m.GetDescriptor()->field_count();
                            const google::protobuf::Reflection* __ref = __m.GetReflection();
                            // printf("__fc=%d\n", __fc);
                            for (int __x = 0; __x < __fc; __x++) {
                                const google::protobuf::FieldDescriptor * __f = __m.GetDescriptor()->field(__x);
                                const int32_t __val = __ref->GetInt32(__m, __f);
                                printf("value=%d\n", __val);
                            }
                        }
                        break;
                        default:
                        {
                            ; // only adapt to test
                        }
                    }
                }
            }
        }

    }

    return 0;
}