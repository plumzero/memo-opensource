
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <string>
#include <exception>

#include "seventh.pb.h"

void opaque_print_values(const google::protobuf::Message& msg)
{
    const std::string tn = msg.GetDescriptor()->name();
    const int fc = msg.GetDescriptor()->field_count();
    const google::protobuf::Reflection* ref = msg.GetReflection();

    int i;

    for (i = 0; i < fc; i++)
    {
        const google::protobuf::FieldDescriptor * f = msg.GetDescriptor()->field(i);    // 如果 field 是 repeated, 那么其 type 以元素类型为准
        switch (f->type())
        {
            case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
            {
                if (f->is_repeated()) {
                    printf("array double: ");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        double __val = ref->GetRepeatedDouble(msg, f, __i);
                        printf("%f ", __val);
                    }
                    printf("\n");
                } else {
                    double __val = ref->GetDouble(msg, f);
                    printf("single double: %f\n", __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
            {
                if (f->is_repeated()) {
                    printf("array float: ");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        double __val = ref->GetRepeatedFloat(msg, f, __i);
                        printf("%f ", __val);
                    }
                    printf("\n");
                } else {
                    double __val = ref->GetFloat(msg, f);
                    printf("single float: %f\n", __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
            {
                if (f->is_repeated()) {
                    printf("array int64: ");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        int64_t __val = ref->GetRepeatedInt64(msg, f, __i);
                        printf("%ld ", __val);
                    }
                    printf("\n");
                } else {
                    int64_t __val = ref->GetInt64(msg, f);
                    printf("single int64: %ld\n", __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
            {
                if (f->is_repeated()) {
                    printf("array uint64: ");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        uint64_t __val = ref->GetRepeatedUInt64(msg, f, __i);
                        printf("%lu ", __val);
                    }
                    printf("\n");
                } else {
                    uint64_t __val = ref->GetUInt64(msg, f);
                    printf("single uint64: %lu\n", __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
            {
                if (f->is_repeated()) {
                    printf("array int32: ");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        int32_t __val = ref->GetRepeatedInt32(msg, f, __i);
                        printf("%d ", __val);
                    }
                    printf("\n");
                } else {
                    int32_t __val = ref->GetInt32(msg, f);
                    printf("single int32: %d\n", __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
            {
                if (f->is_repeated()) {
                    printf("array uint32: ");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        uint32_t __val = ref->GetRepeatedUInt32(msg, f, __i);
                        printf("%u ", __val);
                    }
                    printf("\n");
                } else {
                    uint32_t __val = ref->GetUInt32(msg, f);
                    printf("single uint32: %u\n", __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
            {
                if (f->is_repeated()) {
                    printf("array bool: ");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        bool __val = ref->GetRepeatedBool(msg, f, __i);
                        printf("%s ", __val ? "true" : "false");
                    }
                    printf("\n");
                } else {
                    bool __val = ref->GetBool(msg, f);
                    printf("single bool: %s\n", __val ? "true" : "false");
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                if (f->is_repeated()) {
                    printf("array string: ");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        std::string __val = ref->GetRepeatedString(msg, f, __i);
                        printf("%s ", __val.c_str());
                    }
                    printf("\n");
                } else {
                    std::string __val = ref->GetString(msg, f);
                    printf("single bool: %s\n", __val.c_str());
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
            {
                if (f->is_map() && f->is_repeated()) {  // map
                    printf("map message:\n");
                    int __size = ref->FieldSize(msg, f);    // size of map
                    for (int __i = 0; __i < __size; __i++) {
                        for (int __k = 0; __k < 2; __k++) { // first as key, second as value
                            const google::protobuf::Message & _msg = ref->GetRepeatedMessage(msg, f, __i);
                            const google::protobuf::FieldDescriptor * _f = _msg.GetDescriptor()->field(__k);
                            switch(_f->type())
                            {
                                case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
                                {
                                    double __pod = _msg.GetReflection()->GetDouble(_msg, _f);
                                    printf("%f", __pod);
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
                                {
                                    float __pod = _msg.GetReflection()->GetFloat(_msg, _f);
                                    printf("%f", __pod);
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
                                {
                                    int64_t __pod = _msg.GetReflection()->GetInt64(_msg, _f);
                                    printf("%ld", __pod);
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
                                {
                                    uint64_t __pod = _msg.GetReflection()->GetUInt64(_msg, _f);
                                    printf("%lu", __pod);
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
                                {
                                    int32_t __pod = _msg.GetReflection()->GetInt32(_msg, _f);
                                    printf("%d", __pod);
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
                                {
                                    uint32_t __pod = _msg.GetReflection()->GetUInt32(_msg, _f);
                                    printf("%u", __pod);
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
                                {
                                    bool __pod = _msg.GetReflection()->GetBool(_msg, _f);
                                    printf("%s", __pod ? "true" : "false");
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
                                {
                                    std::string __str = _msg.GetReflection()->GetString(_msg, _f);
                                    printf("%s", __str.c_str());
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
                                {
                                    const google::protobuf::Message & __msg = _msg.GetReflection()->GetMessage(_msg, _f);
                                    opaque_print_values(__msg);
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
                                {
                                    // dose it make some sense?
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
                                {
                                    const int32_t __pod = _msg.GetReflection()->GetEnumValue(_msg, _f);
                                    printf("%d", __pod);
                                }
                                break;
                                default:
                                {
                                    printf("%d unexcepted type\n", __LINE__);
                                    throw std::string("unexcepted type");
                                }
                            }
                            printf("%s", __k ? "\n" : ":");
                        }
                    }
                } else if (f->is_repeated()) {          // repeated struct
                    printf("array message:\n");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        const google::protobuf::Message & __msg = ref->GetRepeatedMessage(msg, f, __i);
                        opaque_print_values(__msg);
                    }
                } else if (f->is_optional()) {          // ordinary struct
                    printf("single message:\n");
                    const google::protobuf::Message & __msg = ref->GetMessage(msg, f);
                    opaque_print_values(__msg);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
            {
                if (f->is_repeated()) {
                    // it doesn't make any sense.
                } else {
                    const std::string bs = ref->GetString(msg, f);
                    printf("bytes: ");
                    for (size_t ti = 0; ti < bs.size(); ti++) {
                        printf(" %02x", static_cast<int>((unsigned char)bs[ti]));
                    }
                    printf("\n");
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
            {
                if (f->is_repeated()) {
                    // it doesn't make any sense.
                } else {
                    int32_t e = ref->GetEnumValue(msg, f);
                    printf("enum: %d\n", e);
                }
            }
            break;
            default:
            {
                printf("%d unexcepted type\n", __LINE__);
                throw std::string("unexcepted type");
            }
        }
    }

    return ;
}

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    mam::Digimon msg;
    
    msg.set_x_d(3.14159);
    msg.set_x_f(2.71828);
    msg.set_x_i64(-123456789123);
    msg.set_x_ui64(123456789123);
    msg.set_x_i32(-123456789);
    msg.set_x_ui32(123456789);
    msg.set_x_bl(true);
    msg.set_x_str("Hello World");
    msg.set_x_enum(mam::ShouldType::TimeType);
    msg.set_x_bs(std::string{ '\x01', '\x00', '\x25', '\xaf', '\x34' });

    msg.mutable_x_map()->insert(google::protobuf::MapPair<std::string, double>("Pi", 3.14159));
    msg.mutable_x_map()->insert(google::protobuf::MapPair<std::string, double>("Golden", 0.618));

    msg.mutable_st()->set_y_i32(128);
    msg.mutable_st()->set_y_i32(256);

    msg.mutable_x_ia()->Add(234);
    msg.mutable_x_ia()->Add(345);
    msg.mutable_x_ia()->Add(456);

    opaque_print_values(msg);

    return 0;
}
