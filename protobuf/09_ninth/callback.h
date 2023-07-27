
#include <cstdio>
#include <stdexcept>

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>

void serialize_callback(const google::protobuf::Message& msg)
{
    const std::string tn = msg.GetDescriptor()->name();
    const int fc = msg.GetDescriptor()->field_count();
    const google::protobuf::Reflection* ref = msg.GetReflection();

    for (int i = 0; i < fc; i++)
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
                    printf("%s:%f,", f->name().c_str(), __val);
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
                    printf("%s:%f,", f->name().c_str(), __val);
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
                    printf("%s:%ld,", f->name().c_str(), __val);
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
                    printf("%s:%lu,", f->name().c_str(), __val);
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
                    printf("%s:%d,", f->name().c_str(), __val);
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
                    printf("%s:%u,", f->name().c_str(), __val);
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
                    printf("%s:%s,", f->name().c_str(), __val ? "true" : "false");
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
                    printf("%s:%s,", f->name().c_str(), __val.c_str());
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
            {
                if (f->is_map() && f->is_repeated()) {  // map
                    // printf("map message:\n");
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
                                    serialize_callback(__msg);
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
                    // printf("array message:\n");
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        const google::protobuf::Message & __msg = ref->GetRepeatedMessage(msg, f, __i);
                        serialize_callback(__msg);
                    }
                } else if (f->is_optional()) {          // ordinary struct
                    printf("single message:\n");
                    const google::protobuf::Message & __msg = ref->GetMessage(msg, f);
                    serialize_callback(__msg);
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
                    int32_t __val = ref->GetEnumValue(msg, f);
                    printf("%s:%d,", f->name().c_str(), __val);
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

    printf("\n");

    return ;
}