
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
 * 原示例版(值得参考)
 * 
 * 测试目的:
 *      1. 分多次写入磁盘
 *      2. 一次从磁盘中读取多处片段
 */
int main()
{
    int i, j;
    
    {   
        // 文件句柄
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_TRUNC);
        
        // 定义数据空间属性(填充 0)
        int fillvalue = 0;
        H5::DSetCreatPropList plist;
        plist.setFillValue(H5::PredType::NATIVE_INT, &fillvalue);
        
        // 数据空间: 确定存储方式(为二维格式)
        hsize_t fdim[] = { FSPACE_DIM1, FSPACE_DIM2 };
        H5::DataSpace fspace(FSPACE_RANK, fdim);
        /** 创建数据集句柄: 存储二维整数(NATIVE_INT)数组 */
        H5::DataSet * dataset = new H5::DataSet(file->createDataSet(DATASET_NAME, H5::PredType::NATIVE_INT, fspace, plist));
        
        // 定义元信息: 数据偏移量，数据对象步长，数据对象数量，原始数据大小
        hsize_t offset[2];
        hsize_t stride[2];
        hsize_t count[2];
        hsize_t block[2];
        offset[0] = 0;  offset[1] = 1;
        stride[0] = 4;  stride[1] = 3;
        count[0]  = 2;  count[1]  = 4;
        block[0]  = 3;  block[1]  = 2;
        // 以矩阵方式选择数据空间
        fspace.selectHyperslab(H5S_SELECT_SET, count, offset, stride, block);

        // 数据集元信息: 一维数组存储为二维格式
        hsize_t mdim1[] = { MSPACE1_DIM };
        H5::DataSpace mspace1(MSPACE1_RANK, mdim1);
        
        // 数据集存储方式
        offset[0] = 1;
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
        
        // 清除原来的对数据空间的选择信息
        fspace.selectNone();
        
        // 再次写入
        hsize_t mdim2[] = { MSPACE2_DIM };
        H5::DataSpace mspace2(MSPACE2_RANK, mdim2);

        hsize_t coord[NPOINTS][FSPACE_RANK];

        coord[0][0] = 0; coord[0][1] = 0;
        coord[1][0] = 3; coord[1][1] = 3;
        coord[2][0] = 3; coord[2][1] = 5;
        coord[3][0] = 5; coord[3][1] = 6;
        // 以点的方式选择数据(空间)
        fspace.selectElements(H5S_SELECT_SET, NPOINTS, (const hsize_t *)coord);

        // 向磁盘写入第二块数据集
        int values[] = { 53, 59, 61, 67 };
        dataset->write(values, H5::PredType::NATIVE_INT, mspace2, fspace);
        
        delete dataset;
        delete file;
    }

    {
        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_RDONLY);

        H5::DataSet * dataset = new H5::DataSet(file->openDataSet(DATASET_NAME));

        H5::DataSpace fspace = dataset->getSpace();

        hsize_t offset[2];
        hsize_t stride[2];
        hsize_t count[2];
        hsize_t block[2];
        
        /// 截取数据空间片段
        offset[0] = 1; offset[1] = 2;
        block[0]  = 1; block[1]  = 1;
        stride[0] = 1; stride[1] = 1;
        count[0]  = 3; count[1]  = 4;
        fspace.selectHyperslab(H5S_SELECT_SET, count, offset, stride, block);

        // 再次截取数据片段，并与第一次截取的结果进行 或运算
        offset[0] = 2; offset[1] = 4;
        block[0]  = 1; block[1]  = 1;
        stride[0] = 1; stride[1] = 1;
        count[0]  = 6; count[1]  = 5;
        fspace.selectHyperslab(H5S_SELECT_OR, count, offset, stride, block);

        hsize_t mdim[] = { MSPACE_DIM1, MSPACE_DIM2 };
        H5::DataSpace mspace(MSPACE_RANK, mdim);

        // 映射到内存中的位置
        offset[0] = 0; offset[1] = 0;
        block[0]  = 1; block[1]  = 1;
        stride[0] = 1; stride[1] = 1;
        count[0]  = 3; count[1]  = 4;
        mspace.selectHyperslab(H5S_SELECT_SET, count, offset, stride, block);
        offset[0] = 1; offset[1] = 2;
        block[0]  = 1; block[1]  = 1;
        stride[0] = 1; stride[1] = 1;
        count[0]  = 6; count[1]  = 5;
        mspace.selectHyperslab(H5S_SELECT_OR, count, offset, stride, block);

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