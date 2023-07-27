
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <algorithm>
#include <map>

#include <iostream>

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>

#include <re2/filtered_re2.h>
#include <re2/re2.h>

template<typename S1, class = typename std::enable_if<std::is_convertible<S1, std::string>::value, std::string>::type,
         typename S2, class = typename std::enable_if<std::is_convertible<S2, std::string>::value, std::string>::type>
bool strnocasecmp(S1 s1, S2 s2)
{
    const std::string & l = s1;
    const std::string & r = s2;
    
    if (l.size() != r.size()) {
        return false;
    }
    
    return std::equal(l.begin(), l.end(), r.begin(), [](const unsigned char & c1, const unsigned char & c2) -> bool {
        return std::tolower(c1) == std::tolower(c2); 
    });
}

std::map<std::string, std::string> split_to_map(const std::string& str, char fieldsep = ',', char kvsep = ':')
{
    std::map<std::string, std::string> res;
    auto start = str.begin();
    auto end = str.end();
    auto next = std::find(start, end, fieldsep);
    
    while (next != end)
    {
        if (start < next) {
            auto mid = std::find(start, next, kvsep);
            if (start != mid && mid != next) {
                res.insert(std::make_pair(std::string(start, mid), std::string(mid + 1, next)));
            }
        }

        start = next + 1;
        next = std::find(start, end, fieldsep);
    }

    return res;
}

bool check_callback(const google::protobuf::Message& msg)
{
    const std::string tn = msg.GetDescriptor()->name();
    const int fc = msg.GetDescriptor()->field_count();
    const google::protobuf::Reflection* ref = msg.GetReflection();

    for (int i = 0; i < fc; i++)
    {
        const google::protobuf::FieldDescriptor * f = msg.GetDescriptor()->field(i);
        switch (f->type())
        {
            case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
            {
                int32_t _as_key = f->options().GetExtension(mam::knightrule).int32_as_key();
                if (_as_key > 0) {
                    printf("====> %s as key\n", f->name().c_str());
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
            {
                uint32_t _val = ref->GetUInt32(msg, f);
                (void) _val;

                uint32_t _rule_pod_val_max = f->options().GetExtension(mam::knightrule).pod_val_max();
                if (_rule_pod_val_max > 0) {
                    // check
                }

                uint32_t _rule_pod_val_min = f->options().GetExtension(mam::knightrule).pod_val_min();
                if (_rule_pod_val_min > 0) {
                    // check
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                std::string _val = ref->GetString(msg, f);
                (void) _val;

                std::string _as_key = f->options().GetExtension(mam::knightrule).string_as_key();
                if (! _as_key.empty()) {
                    printf("====> %s as key\n", f->name().c_str());
                }

                std::string _rule_regex = f->options().GetExtension(mam::knightrule).regex();
 
                if (! _rule_regex.empty() && ! RE2::FullMatch(_val, _rule_regex.c_str())) {
                    printf("-REASON- regex match failed #%s# #%s#\n", _rule_regex.c_str(), _val.c_str());
                    return false;
                }

                uint32_t _rule_str_len_max = f->options().GetExtension(mam::knightrule).str_len_max();
                if (_rule_str_len_max > 0) {
                    // check
                }

                uint32_t _rule_str_len_min = f->options().GetExtension(mam::knightrule).str_len_min();
                if (_rule_str_len_min > 0) {
                    // check
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
            case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
            case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
            case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
            case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
            case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
            case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
            {
                // not check this type temporarily
            }
            break;
            default:
            {
                printf("%d unexcepted type\n", __LINE__);
                throw std::string("unexcepted type");
            }
        }
    }

    return true;
}

void serialize_callback(const google::protobuf::Message& msg)
{
    const std::string tn = msg.GetDescriptor()->name();
    const int fc = msg.GetDescriptor()->field_count();
    const google::protobuf::Reflection* ref = msg.GetReflection();

    for (int i = 0; i < fc; i++)
    {
        const google::protobuf::FieldDescriptor * f = msg.GetDescriptor()->field(i);
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

template <typename T, class = typename std::enable_if<std::is_base_of<google::protobuf::Message, T>::value>::type>
T structuralize_callback(const std::string& line, char fieldsep = ',', char kvsep = ':')
{
    T t;

    std::map<std::string, std::string> _m = split_to_map(line, fieldsep, kvsep);
    
    int count = T::GetDescriptor()->field_count();
    const google::protobuf::Reflection* ref = T::GetReflection();

    for (int i = 0; i < count; i++) {
        const google::protobuf::FieldDescriptor * f = T::GetDescriptor()->field(i);
        if (_m.find(f->name()) == _m.end()) {
            continue;
        }
        switch (f->type())
        {
            case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
            {
                ref->SetDouble(&t, f, std::strtod(_m[f->name()].c_str(), nullptr));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
            {
                ref->SetFloat(&t, f, std::strtof(_m[f->name()].c_str(), nullptr));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
            {
                ref->SetInt64(&t, f, std::strtoll(_m[f->name()].c_str(), nullptr, 0));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
            {
                ref->SetUInt64(&t, f, std::strtoull(_m[f->name()].c_str(), nullptr, 0));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
            {
                ref->SetInt32(&t, f, std::atoi(_m[f->name()].c_str()));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
            {
                ref->SetUInt32(&t, f, static_cast<uint32_t>(std::atoi(_m[f->name()].c_str())));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                ref->SetString(&t, f, _m[f->name()]);
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
            {
                ref->SetBool(&t, f, std::atoi(_m[f->name()].c_str()) ? true : false);
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
            {
                ref->SetEnumValue(&t, f, std::atoi(_m[f->name()].c_str()));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
            case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
            {
                // not check this type temporarily
            }
            break;
            default:
            {
                printf("%d unexcepted type\n", __LINE__);
                throw std::string("unexcepted type");
            }
        }
    }

    return t;
}
