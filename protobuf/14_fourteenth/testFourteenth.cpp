
#include <cstdio>
#include <cassert>
#include <cmath>
#include <string>
#include <iostream>

#include "fourteenth.pb.h"
#include <google/protobuf/arena.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection.h>


#define CTRL_EPS 0.000001

template <typename V>
bool PodCompare(const V _val, const google::protobuf::FieldDescriptor * f)
{
    double _eq = f->options().GetExtension(mam::xrule).eq();
    if (fabs(_eq) >= CTRL_EPS) {
        assert(fabs(_val - _eq) <= CTRL_EPS);
        if (fabs(_val - _eq) <= CTRL_EPS) {
            std::cout << _val << " equal to " << _eq << " ";
        } else {
            return false;
        }
    }


    double _ne = f->options().GetExtension(mam::xrule).ne();
    if (fabs(_ne) >= CTRL_EPS) {
        assert(fabs(_val - _ne) > CTRL_EPS);
        if (fabs(_val - _ne) > CTRL_EPS) {
            std::cout << _val << " not equal to " << _ne << " ";
        } else {
            return false;
        }
    }

    double _gt = f->options().GetExtension(mam::xrule).gt();
    if (fabs(_gt) >= CTRL_EPS) {
        if (fabs(_gt) == CTRL_EPS) {
            assert(_val > fabs(_gt));
            if (_val > fabs(_gt)) {
                std::cout << _val << " greater than " << _gt << " ";
            } else {
                return false;
            }
        } else {
            assert(_val - _gt > 0.0f);
            if (_val - _gt > 0.0f) {
                std::cout << _val << " greater than " << _gt << " ";
            } else {
                return false;
            }
        }
    }

    double _ge = f->options().GetExtension(mam::xrule).ge();
    if (fabs(_ge) >= CTRL_EPS) {
        if (fabs(_ge) == CTRL_EPS) {
            assert(_val >= 0.0f);
            if (_val >= 0.0f) {
                std::cout << _val << " greater equal than " << _ge << " ";
            } else {
                return false;
            }
        } else {
            assert(_val - _ge >= 0.0f);
            if (_val - _ge >= 0.0f) {
                std::cout << _val << " greater equal than " << _ge << " ";
            } else {
                return false;
            }
        }
    }

    double _lt = f->options().GetExtension(mam::xrule).lt();
    if (fabs(_lt) >= CTRL_EPS) {
        if (fabs(_lt) == CTRL_EPS) {
            assert(_val < -0.0f);
            if (_val < -0.0f) {
                std::cout << _val << " less than " << _lt << " ";
            } else {
                return false;
            }
        } else {
            assert(_val - _lt < 0.0f);
            if (_val - _lt < 0.0f) {
                std::cout << _val << " less than " << _lt << " ";
            } else {
                return false;
            }
        }
    }

    double _le = f->options().GetExtension(mam::xrule).le();
    if (fabs(_le) >= CTRL_EPS) {
        if (fabs(_le) == CTRL_EPS) {
            assert(_val <= 0.0f);
            if (_val <= 0.0f) {
                std::cout << _val << " less equal than " << _le << " ";
            } else {
                return false;
            }
        } else {
            assert(_val - _le <= 0.0f);
            if (_val - _le <= 0.0f) {
                std::cout << _val << " less equal than " << _le << " ";
            } else {
                return false;
            }
        }
    }

    std::cout << std::endl;

    return true;
}

template <>
bool PodCompare(const std::string _val, const google::protobuf::FieldDescriptor * f)
{
    std::string _str_in = f->options().GetExtension(mam::xrule).str_in();
    if (! _str_in.empty() && ! _val.empty()) {
        assert(_val.find(_str_in) != std::string::npos);
        if (_val.find(_str_in) != std::string::npos) {
            printf("find %s in %s\n", _str_in.c_str(), _val.c_str());
        } else {
            return false;
        }
    }

    std::string _str_nin = f->options().GetExtension(mam::xrule).str_nin();
    if (! _str_nin.empty() && ! _val.empty()) {
        assert(_val.find(_str_nin) == std::string::npos);
        if (_val.find(_str_nin) == std::string::npos) {
            printf("not find %s in %s\n", _str_nin.c_str(), _val.c_str());
        } else {
            return false;
        }
    }

    std::string _str_eq = f->options().GetExtension(mam::xrule).str_eq();
    if (! _str_eq.empty() && ! _val.empty()) {
        assert(_str_eq == _val);
        if (_str_eq == _val) {
            printf("%s equal to %s\n", _str_eq.c_str(), _val.c_str());
        } else {
            return false;
        }
    }

    std::string _str_ne = f->options().GetExtension(mam::xrule).str_ne();
    if (! _str_ne.empty() && ! _val.empty()) {
        assert(_str_ne != _val);
        if (_str_ne != _val) {
            printf("%s not equal to %s\n", _str_ne.c_str(), _val.c_str());
        } else {
            return false;
        }
    }

    return true;
}

void show(const google::protobuf::Message& msg)
{
    const google::protobuf::Descriptor * des = msg.GetDescriptor();
    const google::protobuf::Reflection * ref = msg.GetReflection();

    // printf("typename=%s,fieldcount=%d\n", des->name().c_str(), des->field_count());

    const int fc = des->field_count();

    for (int i = 0; i < fc; i++) {
        const google::protobuf::FieldDescriptor * f = des->field(i);
        switch (f->type())
        {
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT32:
            {
                uint32_t _val = ref->GetUInt32(msg, f);
                
                assert(PodCompare(_val, f));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT32:
            {
                int32_t _val = ref->GetInt32(msg, f);
                
                assert(PodCompare(_val, f));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_INT64:
            {
                int64_t _val = ref->GetInt64(msg, f);
                
                assert(PodCompare(_val, f));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_UINT64:
            {
                uint64_t _val = ref->GetUInt64(msg, f);
                
                assert(PodCompare(_val, f));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_FLOAT:
            {
                float _val = ref->GetFloat(msg, f);
                
                assert(PodCompare(_val, f));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE:
            {
                double _val = ref->GetDouble(msg, f);
                
                assert(PodCompare(_val, f));
            }
            break;
            case google::protobuf::FieldDescriptor::Type::TYPE_STRING:
            {
                std::string _val = ref->GetString(msg, f);

                assert(PodCompare(_val, f));
            }
            break;
            default:
            {
                printf("unexcepted type(%d)", f->type());
            }
        }
    }
}


int main()
{
    // 字符串测试
    {
        printf("=====> test about string <=====\n");
        mam::SceneString scene;
        
        scene.set_iams_in("x-CRE");     // x
        scene.set_iams_ins("WORKer");   // er
        scene.set_iams_nin("xYz");      // y
        scene.set_iams_nins("linuXR");  // xr

        scene.set_iams_eq("l");
        scene.set_iams_eqs("li");
        scene.set_iams_eqss("hello world");
        scene.set_iams_ne("O");
        scene.set_iams_nes("Oo");
        scene.set_iams_ness("Grok It");

        scene.set_iams_in_symbol("lu_mian");
        scene.set_iams_nin_symbol("lu-mian");

        show(scene);
    }

    // int32 类型测试
    {
        printf("=====> test about int32 <=====\n");

        mam::SceneInt scene;

        scene.set_iami_eq(1980);
        scene.set_iami_ne(-8720);
        scene.set_iami_gt(1785);
        scene.set_iami_ge(-3029);
        scene.set_iami_lt(-8655);
        scene.set_iami_le(99999998);
        scene.set_iami_ge_lt(1200);

        scene.set_iami_eq_zero(0);
        scene.set_iami_ne_zero(1);
        scene.set_iami_gt_zero(1);
        scene.set_iami_ge_zero(0);
        scene.set_iami_lt_zero(-1);
        scene.set_iami_le_zero(0);

        scene.set_iami_ge_lt_zero_left(0);
        scene.set_iami_ge_lt_zero_right(-1);

        show(scene);
    }

    // uint32 类型测试
    {
        printf("=====> test about uint32 <=====\n");

        mam::SceneUInt scene;

        scene.set_iamui_eq(1980);
        scene.set_iamui_ne(8720);
        scene.set_iamui_gt(1785);
        scene.set_iamui_ge(3029);
        scene.set_iamui_lt(8653);
        scene.set_iamui_le(99999999);
        scene.set_iamui_ge_lt(1200);

        scene.set_iamui_eq_zero(0);
        scene.set_iamui_ne_zero(1);
        scene.set_iamui_gt_zero(1);
        scene.set_iamui_ge_zero(0);
        scene.set_iamui_le_zero(0);

        scene.set_iamui_ge_lt_zero_left(0);

        show(scene);
    }

    // int64 类型测试
    {
        printf("=====> test about int64 <=====\n");

        mam::SceneLong scene;

        scene.set_iaml_eq(1980);
        scene.set_iaml_ne(-8720);
        scene.set_iaml_gt(1785);
        scene.set_iaml_ge(-3029);
        scene.set_iaml_lt(-8655);
        scene.set_iaml_le(99999998);
        scene.set_iaml_ge_lt(1200);

        scene.set_iaml_eq_zero(0);
        scene.set_iaml_ne_zero(1);
        scene.set_iaml_gt_zero(1);
        scene.set_iaml_ge_zero(0);
        scene.set_iaml_lt_zero(-1);
        scene.set_iaml_le_zero(0);

        scene.set_iaml_ge_lt_zero_left(0);
        scene.set_iaml_ge_lt_zero_right(-1);

        show(scene);
    }

    // uint64 类型测试
    {
        printf("=====> test about uint64 <=====\n");

        mam::SceneULong scene;

        scene.set_iamul_eq(1980);
        scene.set_iamul_ne(8720);
        scene.set_iamul_gt(1785);
        scene.set_iamul_ge(3029);
        scene.set_iamul_lt(8653);
        scene.set_iamul_le(99999999);
        scene.set_iamul_ge_lt(1200);

        scene.set_iamul_eq_zero(0);
        scene.set_iamul_ne_zero(1);
        scene.set_iamul_gt_zero(1);
        scene.set_iamul_ge_zero(0);
        scene.set_iamul_le_zero(0);

        scene.set_iamul_ge_lt_zero_left(0);

        // show(scene);
    }

    // float 类型测试
    {
        // 经测试对 float32 支持度不好，强烈不建议使用

        // printf("=====> test about float <=====\n");

        // mam::SceneFloat scene;

        // scene.set_iamf_eq(1980.123);
        // scene.set_iamf_ne(-8720.457);
        // scene.set_iamf_gt(1784.790);
        // scene.set_iamf_ge(-3029.123);
        // scene.set_iamf_lt(-8654.457);
        // scene.set_iamf_le(99999999.789);
        // scene.set_iamf_ge_lt(1200.123);

        // scene.set_iamf_eq_zero(0);
        // scene.set_iamf_ne_zero(0.0001);
        // scene.set_iamf_gt_zero(0.00002);
        // scene.set_iamf_ge_zero(0);
        // scene.set_iamf_lt_zero(-0.00001);
        // scene.set_iamf_le_zero(0);

        // scene.set_iamf_ge_lt_zero_left(0.00008);
        // scene.set_iamf_ge_lt_zero_right(-0.000752);

        // show(scene);

    }

    // double 类型测试
    {
        printf("=====> test about double <=====\n");

        mam::SceneDouble scene;

        scene.set_iamd_eq(1980.123);
        scene.set_iamd_ne(-8720.457);
        scene.set_iamd_gt(1784.790);
        scene.set_iamd_ge(-3029.123);
        scene.set_iamd_lt(-8654.457);
        scene.set_iamd_le(99999999.789);
        scene.set_iamd_ge_lt(1200.123);

        scene.set_iamd_eq_zero(0);
        scene.set_iamd_ne_zero(0.0001);
        scene.set_iamd_gt_zero(0.00002);
        scene.set_iamd_ge_zero(0);
        scene.set_iamd_lt_zero(-0.00001);
        scene.set_iamd_le_zero(0);

        scene.set_iamd_ge_lt_zero_left(0.00008);
        scene.set_iamd_ge_lt_zero_right(-0.000752);

        show(scene);
    }

    {
        printf("=====> test about other <=====\n");

        mam::SceneOther scene;

        scene.set_rate(0.99);
        scene.set_server(10);

        show(scene);
    }

    return 0;
}
