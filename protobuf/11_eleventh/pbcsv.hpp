#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <map>

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>

#define CSVLINEBUF 1024
#define LOGERRO printf

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

    if (start < next) {
        auto mid = std::find(start, next, kvsep);
        if (start != mid && mid != next) {
            res.insert(std::make_pair(std::string(start, mid), std::string(mid + 1, next)));
        }
    }

    return res;
}

std::string pb2csv(const google::protobuf::Message& msg)
{
    const std::string tn = msg.GetDescriptor()->name();
    const int fc = msg.GetDescriptor()->field_count();
    const google::protobuf::Reflection* ref = msg.GetReflection();

    char buf[CSVLINEBUF] = { 0 };
    int offset = 0;

    for (int i = 0; i < fc; i++)
    {
        const google::protobuf::FieldDescriptor * f = msg.GetDescriptor()->field(i);
        switch (f->type())
        {
            case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
            {
                if (f->is_repeated()) {
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        double __val = ref->GetRepeatedDouble(msg, f, __i);
                        (void) __val;
                    }
                } else {
                    double __val = ref->GetDouble(msg, f);
                    offset += snprintf(buf + offset, CSVLINEBUF - offset, "%s:%f,", f->name().c_str(), __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
            {
                if (f->is_repeated()) {
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        double __val = ref->GetRepeatedFloat(msg, f, __i);
                        (void) __val;
                    }
                } else {
                    double __val = ref->GetFloat(msg, f);
                    offset += snprintf(buf + offset, CSVLINEBUF - offset, "%s:%f,", f->name().c_str(), __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
            {
                if (f->is_repeated()) {
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        int64_t __val = ref->GetRepeatedInt64(msg, f, __i);
                        (void) __val;
                    }
                } else {
                    int64_t __val = ref->GetInt64(msg, f);
                    offset += snprintf(buf + offset, CSVLINEBUF - offset, "%s:%ld,", f->name().c_str(), __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
            {
                if (f->is_repeated()) {
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        uint64_t __val = ref->GetRepeatedUInt64(msg, f, __i);
                        (void) __val;
                    }
                } else {
                    uint64_t __val = ref->GetUInt64(msg, f);
                    offset += snprintf(buf + offset, CSVLINEBUF - offset, "%s:%lu,", f->name().c_str(), __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
            {
                if (f->is_repeated()) {
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        int32_t __val = ref->GetRepeatedInt32(msg, f, __i);
                        (void) __val;
                    }
                } else {
                    int32_t __val = ref->GetInt32(msg, f);
                    offset += snprintf(buf + offset, CSVLINEBUF - offset, "%s:%d,", f->name().c_str(), __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
            {
                if (f->is_repeated()) {
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        uint32_t __val = ref->GetRepeatedUInt32(msg, f, __i);
                        (void) __val;
                    }
                } else {
                    uint32_t __val = ref->GetUInt32(msg, f);
                    offset += snprintf(buf + offset, CSVLINEBUF - offset, "%s:%u,", f->name().c_str(), __val);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
            {
                if (f->is_repeated()) {
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        bool __val = ref->GetRepeatedBool(msg, f, __i);
                        (void) __val;
                    }
                } else {
                    bool __val = ref->GetBool(msg, f);
                    offset += snprintf(buf + offset, CSVLINEBUF - offset, "%s:%s,", f->name().c_str(), __val ? "true" : "false");
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                if (f->is_repeated()) {
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        std::string __val = ref->GetRepeatedString(msg, f, __i);
                        (void) __val;
                    }
                } else {
                    std::string __val = ref->GetString(msg, f);
                    offset += snprintf(buf + offset, CSVLINEBUF - offset, "%s:%s,", f->name().c_str(), __val.c_str());
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
            {
                if (f->is_map() && f->is_repeated()) {  // map
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
                                    (void) __pod;
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
                                {
                                    float __pod = _msg.GetReflection()->GetFloat(_msg, _f);
                                    (void) __pod;
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
                                {
                                    int64_t __pod = _msg.GetReflection()->GetInt64(_msg, _f);
                                    (void) __pod;
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
                                {
                                    uint64_t __pod = _msg.GetReflection()->GetUInt64(_msg, _f);
                                    (void) __pod;
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
                                {
                                    int32_t __pod = _msg.GetReflection()->GetInt32(_msg, _f);
                                    (void) __pod;
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
                                {
                                    uint32_t __pod = _msg.GetReflection()->GetUInt32(_msg, _f);
                                    (void) __pod;
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
                                {
                                    bool __pod = _msg.GetReflection()->GetBool(_msg, _f);
                                    (void) __pod;
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
                                {
                                    std::string __str = _msg.GetReflection()->GetString(_msg, _f);
                                    (void) __str;
                                }
                                break;
                                case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
                                {
                                    const google::protobuf::Message & __msg = _msg.GetReflection()->GetMessage(_msg, _f);
                                    pb2csv(__msg);
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
                                    (void) __pod;
                                }
                                break;
                                default:
                                {
                                    LOGERRO("unexcepted type(%d)", f->type());
                                }
                            }
                            offset += snprintf(buf + offset, CSVLINEBUF - offset, "%s", __k ? "\n" : ":");
                        }
                    }
                } else if (f->is_repeated()) {          // repeated struct
                    int __size = ref->FieldSize(msg, f);
                    for (int __i = 0; __i < __size; __i++) {
                        const google::protobuf::Message & __msg = ref->GetRepeatedMessage(msg, f, __i);
                        pb2csv(__msg);
                    }
                } else if (f->is_optional()) {          // ordinary struct
                    const google::protobuf::Message & __msg = ref->GetMessage(msg, f);
                    pb2csv(__msg);
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_BYTES:
            {
                if (f->is_repeated()) {
                    // it doesn't make any sense.
                } else {
                    const std::string bs = ref->GetString(msg, f);
                    (void) bs;
                }
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
            {
                if (f->is_repeated()) {
                    // it doesn't make any sense.
                } else {
                    int32_t __val = ref->GetEnumValue(msg, f);
                    (void) __val;
                }
            }
            break;
            default:
            {
                LOGERRO("unexcepted type(%d)", f->type());
            }
        }
    }

    offset += snprintf(buf + offset, CSVLINEBUF - offset, "\n");

    return std::string(buf, buf + offset);
}

template <typename T, class = typename std::enable_if<std::is_base_of<google::protobuf::Message, T>::value>::type>
void csv2pb(T* t, const std::string& line, char fieldsep = ',', char kvsep = ':')
{
    std::map<std::string, std::string> _m = split_to_map(line, fieldsep, kvsep);
    
    int count = t->GetDescriptor()->field_count();
    const google::protobuf::Reflection* ref = t->GetReflection();

    for (int i = 0; i < count; i++) {
        const google::protobuf::FieldDescriptor * f = t->GetDescriptor()->field(i);
        if (_m.find(f->name()) == _m.end()) {
            continue;
        }
        switch (f->type())
        {
            case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
            {
                ref->SetDouble(t, f, std::strtod(_m[f->name()].c_str(), nullptr));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
            {
                ref->SetFloat(t, f, std::strtof(_m[f->name()].c_str(), nullptr));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
            {
                ref->SetInt64(t, f, std::strtoll(_m[f->name()].c_str(), nullptr, 0));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
            {
                ref->SetUInt64(t, f, std::strtoull(_m[f->name()].c_str(), nullptr, 0));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
            {
                ref->SetInt32(t, f, std::atoi(_m[f->name()].c_str()));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
            {
                ref->SetUInt32(t, f, static_cast<uint32_t>(std::atoi(_m[f->name()].c_str())));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                ref->SetString(t, f, _m[f->name()]);
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_BOOL:
            {
                ref->SetBool(t, f, std::atoi(_m[f->name()].c_str()) ? true : false);
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_ENUM:
            {
                ref->SetEnumValue(t, f, std::atoi(_m[f->name()].c_str()));
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
                LOGERRO("unexcepted type(%d)", f->type());
            }
        }
    }

    // return t;
}
