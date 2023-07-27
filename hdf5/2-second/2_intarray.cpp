
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
 * 二维数组写入矩阵及将矩阵片段读入二维数组
 * 
 * 简版
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
        int data[NX][NY];
        for (i = 0; i < NX; i++) {
            for (j = 0; j < NY; j++) {
                data[i][j] = 0;
            }
        }

        H5::H5File file(FILE_NAME, H5F_ACC_RDONLY);
        H5::DataSet dataset = file.openDataSet(DATASET_NAME);

        H5::DataSpace fspace = dataset.getSpace();

        // 选择片段
        hsize_t offset[2];
        hsize_t count[2];
        offset[0] = 1;      offset[1] = 2;
        count[0]  = NX_SUB; count[1]  = NY_SUB;
        fspace.selectHyperslab(H5S_SELECT_SET, count, offset);

        // 内存(注意: 内存偏移与读取片段综合后不要超过内存范围)
        hsize_t dimsm[2];
        dimsm[0] = NX;
        dimsm[1] = NY;
        H5::DataSpace memspace(RANK, dimsm);

        // 映射到内存中的位置
        offset[0] = 1;
        offset[1] = 0;
        count[0]  = NX_SUB;
        count[1]  = NY_SUB;
        memspace.selectHyperslab(H5S_SELECT_SET, count, offset);

        // 读取
        dataset.read(data, H5::PredType::NATIVE_INT, memspace, fspace);

        for (i = 0; i < NX; i++) {
            for (j = 0; j < NY; j++) {
                printf("%2d ", data[i][j]);
            }
            printf("\n");
        }
    }

    printf("======== test over ======\n");

    return 0;
}

