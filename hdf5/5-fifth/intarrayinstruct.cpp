
#include <cstdio>
#include "H5Cpp.h"

const H5std_string FILE_NAME("ints.h5");
const H5std_string DATASET_NAME("ArrayofIntInStruct");

const int RANK = 1;     // 一维

typedef struct {
    short   tdim;
    int coll_ids[4];
} TEST_t;

int main()
{
    // 写
    {
        TEST_t tdata = { 102, { 104, 105, 106, 107 } };

        // 文件空间
        hsize_t dim[] = { 1 };  // 长度
        H5::DataSpace fspace(RANK, dim);

        // 文件句柄
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_TRUNC);

        // 为各数据按类型分配内存空间
        hsize_t subdim[] = { 4 };      // sizeof(TEST_t::coll_ids) / sizeof(int)
        H5::ArrayType _AT(H5::PredType::NATIVE_INT, RANK, subdim);      // 第二个参数为维数
        H5::CompType IntArrayType(4 * sizeof(int));
        IntArrayType.insertMember("_", 0, _AT);

        H5::CompType mtype(sizeof(TEST_t));
        mtype.insertMember("tdim", HOFFSET(TEST_t, tdim), H5::PredType::NATIVE_SHORT);
        mtype.insertMember("coll_ids", HOFFSET(TEST_t, coll_ids), IntArrayType);

        // 数据集句柄
        H5::DataSet * dataset = new H5::DataSet(file->createDataSet(DATASET_NAME, mtype, fspace));

        // 写入数据
        dataset->write(&tdata, mtype);

        _AT.close();
        delete dataset;
        delete file;
    }

    // 读
    {
        int i;
        // 文件句柄
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_RDONLY);
        H5::DataSet * dataset = new H5::DataSet(file->openDataSet(DATASET_NAME));

        // 为各数据按类型分配内存空间(定义内存结构模型)
        hsize_t subdim[] = { 4 };      // sizeof(TEST_t::coll_ids) / sizeof(int)
        H5::ArrayType _AT(H5::PredType::NATIVE_INT, RANK, subdim);      // 第二个参数为维数
        H5::CompType IntArrayType(4 * sizeof(int));
        IntArrayType.insertMember("_", 0, _AT);

        H5::CompType mtype(sizeof(TEST_t));
        mtype.insertMember("tdim", HOFFSET(TEST_t, tdim), H5::PredType::NATIVE_SHORT);
        mtype.insertMember("coll_ids", HOFFSET(TEST_t, coll_ids), IntArrayType);

        TEST_t tdata;
        dataset->read(&tdata, mtype);
        
        printf("tdim=%d, coll_ids=( ", tdata.tdim);
        for (i = 0; i < 4; i++) {
            printf("%02d ", tdata.coll_ids[i]);
        }
        printf(")\n");

        _AT.close();
        delete dataset;
        delete file;
    }

    printf("====== test over ======\n");

    return 0;
}