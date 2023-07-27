
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
 * 原示例版(只作参考)
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

        // 定义存储到文件中的数据类型，这里采用小端整型
        H5::IntType datatype(H5::PredType::NATIVE_INT);
        datatype.setOrder(H5T_ORDER_BE);

        H5::DataSet dataset = file.createDataSet(DATASET_NAME, datatype, fspace); // 
        // H5::DataSet dataset = file.createDataSet(DATASET_NAME, H5::PredType::NATIVE_INT, fspace);

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

        H5T_class_t type_class = dataset.getTypeClass();

        if (type_class == H5T_INTEGER)
        {
            printf("Dataset has INTEGER type\n");

            H5::IntType intype = dataset.getIntType();

            H5std_string order_string;
            H5T_order_t order = intype.getOrder(order_string);
            printf("字节序信息: %s\n", order_string.c_str());

            size_t size = intype.getSize();
            printf("Data size is %lu\n", size);
        }

        H5::DataSpace fspace = dataset.getSpace();

        /** 获取数据的维数并展示 */
        int rank = fspace.getSimpleExtentNdims();
        hsize_t dims[2];
        int ndims = fspace.getSimpleExtentDims(dims, NULL);
        printf("rank %d, dimensions %llu x %llu, ndims=%d\n", rank, dims[0], dims[1], ndims);

        /** 截取文件片段 */
        hsize_t offset[2];
        hsize_t count[2];
        offset[0] = 1;
        offset[1] = 2;
        count[0]  = NX_SUB;
        count[1]  = NY_SUB;
        fspace.selectHyperslab(H5S_SELECT_SET, count, offset);

        /** 定义内存矩阵 */
        hsize_t dimsm[2];
        dimsm[0] = NX;
        dimsm[1] = NY;
        H5::DataSpace memspace(RANK, dimsm);

        /** 映射到内存中的位置(hyperslab) */
        offset[0] = 1;  // 注意: 文件偏移与内存映射位置结合后不要超过内存
        offset[1] = 0;
        count[0]  = NX_SUB;
        count[1]  = NY_SUB;
        memspace.selectHyperslab(H5S_SELECT_SET, count, offset);

        /** 将文件 hyperslab 中的数据，映射到内存中的 hyperslab 并展示 */
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

