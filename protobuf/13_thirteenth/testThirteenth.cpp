
#include <cstdio>
#include <cassert>
#include <string>

#include "arenatest.pb.h"
#include <google/protobuf/arena.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>

int main(int argc, char* argv[])
{   
    (void) argc;
    (void) argv;

    mam::Person person;
    
    person.set_name("turnover");
    person.set_id(878362);
    person.set_email("xxx@email.com");

    google::protobuf::Message * temp = person.New(google::protobuf::Arena::GetArena(&person));

    const std::string tn = temp->GetDescriptor()->name();
    const int fc = temp->GetDescriptor()->field_count();
    const google::protobuf::Reflection* ref = temp->GetReflection();

    for (int i = 0; i < fc; i++) {
        const google::protobuf::FieldDescriptor * f = temp->GetDescriptor()->field(i);
        switch(f->type()) {
            case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
            {
                int32_t _val = ref->GetInt32(*temp, f);
                printf("%s=%d\n", f->name().c_str(), _val);
                ref->SetInt32(temp, f, 123);
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                std::string _val = ref->GetString(*temp, f);
                printf("%s=%s\n", f->name().c_str(), _val.c_str());
                ref->SetString(temp, f, "Hello World");
            }
            break;
            default:
            {
                ;
            }
        }
    }

    printf("person: name=%s,id=%d,email=%s\n", person.name().c_str(), person.id(), person.email().c_str());
    for (int i = 0; i < fc; i++) {
        const google::protobuf::FieldDescriptor * f = temp->GetDescriptor()->field(i);
        switch(f->type()) {
            case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
            {
                int32_t _val = ref->GetInt32(*temp, f);
                printf("%s=%d\n", f->name().c_str(), _val);
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                std::string _val = ref->GetString(*temp, f);
                printf("%s=%s\n", f->name().c_str(), _val.c_str());
            }
            break;
            default:
            {
                ;
            }
        }
    }

    return 0;
}
