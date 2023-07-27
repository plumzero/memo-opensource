
#include <string>

#include "H5Cpp.h"

const H5std_string FILE_NAME("SDS.h5");
const H5std_string DATASET_NAME("IntArray");
const int    NX_SUB = 3;        // hyperslab dimensions
const int    NY_SUB = 4;

const int    NX = 5;            // output buffer dimensions
const int    NY = 6;

const int    RANK = 2;

/**
 * 二维数组写入矩阵及将矩阵片段读入一维数组
 * 
 * 扩展版
 * 
 * 测试结果: 失败
 */

int main ()
{
    // 写
    {
        int i, j;
        int data[NX][NY];

        for (i = 0; i < NX; i++) {
            for(j = 0; j < NY; j++) {
                data[i][j] = i + j;
            }
        }

        for (i = 0; i < NX; i++) {
            for(j = 0; j < NY; j++) {
                printf("%2d ", data[i][j]);
            }
            printf("\n");
        }

        H5::H5File file(FILE_NAME, H5F_ACC_TRUNC);

        hsize_t fdim[] = { NX, NY };
        H5::DataSpace fspace(RANK, fdim);

        H5::DataSet dataset = file.createDataSet(DATASET_NAME, H5::PredType::NATIVE_INT, fspace);

        dataset.write(data, H5::PredType::NATIVE_INT);
    }

    // 读
    {
        int i, j;

        int data[NX * NY] = { 0 };

        H5::H5File file(FILE_NAME, H5F_ACC_RDONLY);
        H5::DataSet dataset = file.openDataSet(DATASET_NAME);

        H5::DataSpace fspace = dataset.getSpace();

        // 选择片段
        hsize_t offset[2];
        hsize_t count[2];
        hsize_t stride[2];
        hsize_t block[2];
        offset[0] = 1;         offset[1] = 2;
        count[0]  = NX_SUB;    count[1]  = NY_SUB;
        stride[0] = 1;         stride[1] = 1;
        block[0]  = 1;         block[1]  = 1;
        fspace.selectHyperslab(H5S_SELECT_SET, count, offset);

        // 内存(注意: 内存偏移与读取片段综合后不要超过内存范围)
        hsize_t dimsm[] = { NX * NY + 10 };
        H5::DataSpace memspace(1, dimsm);

        // 映射到内存中的位置(一维数组，线性偏移)
        offset[0] = 0;
        count[0]  = 3;
        stride[0] = 1;
        block[0]  = 1;

        memspace.selectHyperslab(H5S_SELECT_SET, count, offset);

        // 读取
        dataset.read(data, H5::PredType::NATIVE_INT, memspace, fspace);
        printf("---------------------------------\n");
        for (i = 0; i < NX * NY; i++) {
            printf("%2d ", data[i]);
            if ((i + 1) % NX == 0) {
                printf("\n");
            }
        }
    }

    printf("======== test over ======\n");

    return 0;
}

