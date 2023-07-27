
#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#include <algorithm>

#include "sixth.pb.h"

/**
 * 类型描述接口文件: google/protobuf/descriptor.h 
 */

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    mam::ForCheckMsg msg;
    
    msg.set_name("test");
    msg.set_time(123456789);
    
    (*msg.mutable_dic())["name"] = mam::NameType;
    (*msg.mutable_dic())["time"] = mam::TimeType;
    
    printf("name=%s\n", msg.name().c_str());
    printf("time=%d\n", msg.time());
    
    decltype(msg.dic().begin()) it;
    for (it = msg.dic().begin(); it != msg.dic().end(); it++) {
        printf("key=%s, value=%d\n", it->first.c_str(), it->second);
    }
    printf("\n");

    // 通过类型得到(遍历)
    const std::string x_type_full_name = mam::ForCheckMsg::GetDescriptor()->full_name();
    const std::string x_type_name = mam::ForCheckMsg::GetDescriptor()->name();
    printf("type full name: %s\n", x_type_full_name.c_str());
    printf("type      name: %s\n", x_type_name.c_str());

    int x_count = mam::ForCheckMsg::GetDescriptor()->field_count();       // yes
    printf("count of members in struct: %d\n", x_count);

    int xi;
    for (xi = 0; xi < x_count; xi++) {
        const google::protobuf::FieldDescriptor * x_field = mam::ForCheckMsg::GetDescriptor()->field(xi);
        printf("typeid:%-2d typename:%-8s fieldname:%-6s fieldfullname:%s\n", x_field->type(), x_field->type_name(), x_field->name().c_str(), x_field->full_name().c_str());
        if (strcmp(x_field->type_name(), "message") == 0) {
            if (x_field->is_map()) {
                printf("    %s's type is map\n", x_field->name().c_str());
            }
        }
    }
    printf("\n");

    // 通过对象得到(遍历)
    const std::string y_type_full_name = msg.GetDescriptor()->full_name();
    const std::string y_type_name = msg.GetDescriptor()->name();
    printf("type full name: %s\n", y_type_full_name.c_str());
    printf("type      name: %s\n", y_type_name.c_str());
    
    int y_count = mam::ForCheckMsg::GetDescriptor()->field_count();       // yes
    printf("count of members in struct: %d\n", y_count);

    int yi;
    for (yi = 0; yi < y_count; yi++) {
        const google::protobuf::FieldDescriptor * y_field = mam::ForCheckMsg::GetDescriptor()->field(yi);
        printf("typeid:%-2d typename:%-8s fieldname:%-6s fieldfullname:%s\n", y_field->type(), y_field->type_name(), y_field->name().c_str(), y_field->full_name().c_str());
        if (strcmp(y_field->type_name(), "message") == 0) {
            if (y_field->is_map()) {
                printf("    %s's type is map\n", y_field->name().c_str());
            }
        }
    }
    printf("\n");

    // 直接点名
    const google::protobuf::FieldDescriptor * z_field;

    z_field = mam::ForCheckMsg::GetDescriptor()->FindFieldByName("name");
    printf("typeid:%-2d typename:%-8s fieldname:%-6s fieldfullname:%s\n", z_field->type(), z_field->type_name(), z_field->name().c_str(), z_field->full_name().c_str());
    
    z_field = mam::ForCheckMsg::GetDescriptor()->FindFieldByName("time");
    printf("typeid:%-2d typename:%-8s fieldname:%-6s fieldfullname:%s\n", z_field->type(), z_field->type_name(), z_field->name().c_str(), z_field->full_name().c_str());

    z_field = mam::ForCheckMsg::GetDescriptor()->FindFieldByName("dic");
    printf("typeid:%-2d typename:%-8s fieldname:%-6s fieldfullname:%s\n", z_field->type(), z_field->type_name(), z_field->name().c_str(), z_field->full_name().c_str());

    z_field = mam::ForCheckMsg::GetDescriptor()->FindFieldByName("id");
    printf("typeid:%-2d typename:%-8s fieldname:%-6s fieldfullname:%s\n", z_field->type(), z_field->type_name(), z_field->name().c_str(), z_field->full_name().c_str());

    return 0;
}
