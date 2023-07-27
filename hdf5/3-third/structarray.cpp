
#include <cstdio>
#include "H5Cpp.h"

const H5std_string FILE_NAME("SDScompound.h5");
const H5std_string DATASET_NAME("ArrayOfStructures");
const H5std_string MEMBER1("a_name");
const H5std_string MEMBER2("b_name");
const H5std_string MEMBER3("c_name");
const int LENGTH = 10;
const int RANK = 1;

int main()
{
    typedef struct s1_t {
         int    a;
         float  b;
         double c;
    } s1_t;

    typedef struct s2_t {
         double c;
         int    a;
    } s2_t;

    // 写
    {
        int  i;
        s1_t s1[LENGTH];
        for (i = 0; i < LENGTH; i++) {
            s1[i].a = i;
            s1[i].b = i * i;
            s1[i].c = 1. / (i + 1);
        }

        /** 文件空间 */
        hsize_t dim[] = { LENGTH };
        H5::DataSpace fspace(RANK, dim);

        /** 文件句柄 */
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_TRUNC);

        /** 为各数据按类型分配内存空间 */
        H5::CompType mtype1(sizeof(s1_t));
        mtype1.insertMember(MEMBER1, HOFFSET(s1_t, a), H5::PredType::NATIVE_INT);
        mtype1.insertMember(MEMBER3, HOFFSET(s1_t, c), H5::PredType::NATIVE_DOUBLE);
        mtype1.insertMember(MEMBER2, HOFFSET(s1_t, b), H5::PredType::NATIVE_FLOAT);

        /** 数据集句柄 */
        H5::DataSet * dataset = new H5::DataSet(file->createDataSet(DATASET_NAME, mtype1, fspace));

        /** 写入数据 */
        dataset->write(s1, mtype1);

        delete dataset;
        delete file;
    }

    {
        int  i;
        /** 文件句柄 */
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_RDONLY);
        H5::DataSet * dataset = new H5::DataSet(file->openDataSet(DATASET_NAME));

        /** 为各数据按类型分配内存空间(定义内存结构模型) */
        H5::CompType mtype2(sizeof(s2_t));
        mtype2.insertMember(MEMBER3, HOFFSET(s2_t, c), H5::PredType::NATIVE_DOUBLE);
        mtype2.insertMember(MEMBER1, HOFFSET(s2_t, a), H5::PredType::NATIVE_INT);

        /** 将 c 和 a 域从文件空间(通过 c_name 和 a_name 找到)填充到结构中 */
        s2_t s2[LENGTH];
        dataset->read(s2, mtype2);

        /** 展示 */
        printf("Filed c: ");
        for (i = 0; i < LENGTH; i++) {
            printf("%f ", s2[i].c);
        }
        printf("\n");

        printf("Filed a: ");
        for (i = 0; i < LENGTH; i++) {
            printf("%2d ", s2[i].a);
        }
        printf("\n");

        /** 其他测试... */
        H5::CompType mtype3(sizeof(float));

        mtype3.insertMember(MEMBER2, 0, H5::PredType::NATIVE_FLOAT);

        float s3[LENGTH];
        dataset->read(s3, mtype3);

        printf("Filed b: ");
        for (i = 0; i < LENGTH; i++) {
            printf("%f ", s3[i]);
        }
        printf("\n");

        delete dataset;
        delete file;
    }

    printf("====== test over ======\n");

    return 0;
}
