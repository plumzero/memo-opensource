
#include <cstdio>
#include "H5Cpp.h"

const H5std_string FILE_NAME("chars.h5");
const H5std_string DATASET_NAME("ArrayofCharInStruct");

const int RANK = 1;     // 一维

typedef struct {
    char    name[16];
    short   tdim;
} TEST_t;

int main()
{
    // 写
    {
        TEST_t tdata = { "one", 102 };

        // 文件空间
        hsize_t dim[] = { 1 };
        H5::DataSpace fspace(RANK, dim);

        // 文件句柄
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_TRUNC);

        // 为各数据按类型分配内存空间
        H5::StrType CharsType(H5::PredType::C_S1, sizeof(TEST_t::name));
        H5::CompType mtype(sizeof(TEST_t));
        mtype.insertMember("name", HOFFSET(TEST_t, name), CharsType);
        mtype.insertMember("tdim", HOFFSET(TEST_t, tdim), H5::PredType::NATIVE_SHORT);

        // 数据集句柄
        H5::DataSet * dataset = new H5::DataSet(file->createDataSet(DATASET_NAME, mtype, fspace));

        // 写入数据
        dataset->write(&tdata, mtype);

        delete dataset;
        delete file;
    }

    // 读
    {
        // 文件句柄
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_RDONLY);
        H5::DataSet * dataset = new H5::DataSet(file->openDataSet(DATASET_NAME));

        // 为各数据按类型分配内存空间(定义内存结构模型)
        H5::StrType CharsType(H5::PredType::C_S1, sizeof(TEST_t::name));
        H5::CompType mtype(sizeof(TEST_t));
        mtype.insertMember("name", HOFFSET(TEST_t, name), CharsType);
        mtype.insertMember("tdim", HOFFSET(TEST_t, tdim), H5::PredType::NATIVE_SHORT);

        TEST_t tdata;
        dataset->read(&tdata, mtype);
        
        printf("name=%s,tdim=%d\n", tdata.name, tdata.tdim);

        delete dataset;
        delete file;
    }

    printf("====== test over ======\n");

    return 0;
}