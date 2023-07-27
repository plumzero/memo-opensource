
#include <cstdio>
#include <string>

#include "H5Cpp.h"

const H5std_string FILE_NAME("Select.h5");
const H5std_string DATASET_NAME("Matrix in file");

const int MSPACE1_RANK = 1;     // Rank of the first dataset in memory
const int MSPACE1_DIM = 50;     // Dataset size in memory
const int MSPACE2_RANK = 1;     // Rank of the second dataset in memory
const int MSPACE2_DIM = 4;      // Dataset size in memory
const int FSPACE_RANK = 2;      // Dataset rank as it is stored in the file
const int FSPACE_DIM1 = 8;      // Dimension sizes of the dataset as it is
const int FSPACE_DIM2 = 12;     //      stored in the file

const int MSPACE_RANK = 2;      // Rank of the first dataset in memory
const int MSPACE_DIM1 = 8;      // We will read dataset back from the file
const int MSPACE_DIM2 = 9;      //      to the dataset in memory with these dataspace parameters
const int NPOINTS = 4;          // Number of points that will be selected and overwritten

/**
 * 将线性数组元素以矩阵方式进行存储: 3×2块,(4,3)步长,(2,4)数量
 * 
 * 冗余信息版(作为学习)
 * 
 * 测试目的: 
 *      1. 定义数据空间(DataSpace)的元信息
 *      2. 获取数据空间(DataSpace)的元信息
 *      3. 理解写和读是相互独立的
 */
int main()
{
    // 写
    {
        int i, j;

        // 文件句柄
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_TRUNC);
        
        // 定义数据空间属性(填充 0)
        int fillvalue = 0;
        H5::DSetCreatPropList plist;
        plist.setFillValue(H5::PredType::NATIVE_INT, &fillvalue);

        // 数据空间
        hsize_t fdim[] = { FSPACE_DIM1, FSPACE_DIM2 };
        H5::DataSpace fspace(FSPACE_RANK, fdim);
        /** 创建数据集句柄: 存储二维整数(NATIVE_INT)数组 */
        H5::DataSet * dataset = new H5::DataSet(file->createDataSet(DATASET_NAME, H5::PredType::NATIVE_INT, fspace, plist));

        // 定义元信息: 数据偏移量，数据对象步长，数据对象数量，原始数据大小
        // 当然不能随便乱填呀!  数据空间: 8 × 12
        hsize_t offset[2];
        hsize_t stride[2];
        hsize_t count[2];
        hsize_t block[2];
        offset[0] = 0;  offset[1] = 1;  // 存储的偏移位置
        stride[0] = 4;  stride[1] = 3;  // 数据对象矩阵
        count[0]  = 2;  count[1]  = 4;  // 数据对象数量
        block[0]  = 3;  block[1]  = 2;  // 原始数据存储矩阵
        // 设置数据空间元信息
        fspace.selectHyperslab(H5S_SELECT_SET, count, offset, stride, block);

        // 数据集元信息
        hsize_t mdim1[] = { MSPACE1_DIM };
        H5::DataSpace mspace1(MSPACE1_RANK, mdim1);

        // 数据集存储方式
        offset[0] = 1;  // 从数组索引为 1 的位置开始存储
        stride[0] = 1;
        count[0]  = 48;
        block[0]  = 1;
        mspace1.selectHyperslab(H5S_SELECT_SET, count, offset, stride, block);

        // 定义数据集
        int vec[MSPACE1_DIM];
        vec[0] = vec[MSPACE1_DIM - 1] = -1;
        for (i = 1; i < MSPACE1_DIM - 1; i++) {
            vec[i] = i;
        }

        // 数据集写入
        dataset->write(vec, H5::PredType::NATIVE_INT, mspace1, fspace);

        delete dataset;
        delete file;
    }

    // 读
    {
        int i, j;
        // 文件句柄
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_RDONLY);
        
        // 数据集句柄
        H5::DataSet * dataset = new H5::DataSet(file->openDataSet(DATASET_NAME));

        // 获取数据空间
        H5::DataSpace fspace = dataset->getSpace();

        // 简单的数据空间维数信息
        int ndims;
        ndims = fspace.getSimpleExtentNdims();
        if (ndims < 0) {
            printf("DataSpace::getSimpleExtentNdims failed: %d\n", ndims);
            return -1;
        }
        printf("数据空间的维数信息: %d\n", ndims);

        // 数据空间存储数据类型
        H5S_class_t ftype = fspace.getSimpleExtentType();
        switch (ftype) {
            case H5S_NO_CLASS:
                printf("DataSpace::getSimpleExtentType failed\n");
                return -1;
            break;
            case H5S_SCALAR:
                printf("数据空间存储的数据类型为: SCALAR\n");
            break;
            case H5S_SIMPLE:
                printf("数据空间存储的数据类型为: SIMPLE\n");
            break;
            case H5S_NULL:
                printf("数据空间存储的数据类型为: NULL\n");
            break;
        }

        // 数据空间的元素数量(存疑)
        hssize_t neles;
        neles = fspace.getSimpleExtentNpoints();
        if (neles < 0) {
            printf("DataSpace::getSimpleExtentNpoints failed: %lld\n", neles);
            return -1;
        }
        printf("数据空间的元素数量: %lld\n", neles);

        // 选择数据空间中某一片段映射到内存
        hsize_t offset[2];
        hsize_t block[2];
        hsize_t stride[2];
        hsize_t count[2];
        offset[0] = 1; offset[1] = 2;
        block[0]  = 1; block[1]  = 1;
        stride[0] = 1; stride[1] = 1;
        count[0]  = 3; count[1]  = 4;
        fspace.selectHyperslab(H5S_SELECT_SET, count, offset, stride, block);

        // 获取数据空间元信息
        hssize_t nblocks;
        nblocks = fspace.getSelectHyperNblocks();
        if (nblocks < 0) {
            printf("DataSpace::getSelectHyperNblocks failed: %lld\n", nblocks);
            return -1;
        }
        printf("所选择数据空间该片段中块的数量: %lld\n", nblocks);

        // 映射到内存中的位置
        hsize_t mdim[] = { MSPACE_DIM1, MSPACE_DIM2 };
        H5::DataSpace mspace(MSPACE_RANK, mdim);
        offset[0] = 2; offset[1] = 2;
        block[0]  = 1; block[1]  = 1;
        stride[0] = 1; stride[1] = 1;
        count[0]  = 3; count[1]  = 4;
        mspace.selectHyperslab(H5S_SELECT_SET, count, offset, stride, block);

        int matrix_out[MSPACE_DIM1][MSPACE_DIM2];
        for (i = 0; i < MSPACE_DIM1; i++) {
            for (j = 0; j < MSPACE_DIM2; j++) {
                matrix_out[i][j] = 0;
            }
        }

        dataset->read(matrix_out, H5::PredType::NATIVE_INT, mspace, fspace);

        for (i = 0; i < MSPACE_DIM1; i++) {
            for (j = 0; j < MSPACE_DIM2; j++) {
                printf("%2d ", matrix_out[i][j]);
            }
            printf("\n");
        }

        delete dataset;
        delete file;
    }

    printf("====== test over ======\n");

    return 0;
}