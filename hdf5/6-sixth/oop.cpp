
#include <cstdio>
#include <cstring>
#include <string>
#include "H5Cpp.h"

const H5std_string FILE_NAME("OOP.h5");
const H5std_string DATASET_NAME("CppClass");

const int RANK = 1;

class PR
{
public:
    int id;
    char label[10];
    int time;
    double value;
    float result;

    void toChars(std::string & stream)
    {
        (void) stream;

        stream.clear();

        hsize_t dim[] = { 1 };
        H5::DataSpace fspace(RANK, dim);

        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_TRUNC);

        H5::StrType CharsType(H5::PredType::C_S1, sizeof(PR::label));
        H5::CompType mtype(sizeof(PR));
        mtype.insertMember("id",     HOFFSET(PR, id),     H5::PredType::NATIVE_INT);
        mtype.insertMember("label",  HOFFSET(PR, label),  CharsType);
        mtype.insertMember("time",   HOFFSET(PR, time),   H5::PredType::NATIVE_INT);
        mtype.insertMember("value",  HOFFSET(PR, value),  H5::PredType::NATIVE_DOUBLE);
        mtype.insertMember("result", HOFFSET(PR, result), H5::PredType::NATIVE_FLOAT);

        H5::DataSet * dataset = new H5::DataSet(file->createDataSet(DATASET_NAME, mtype, fspace));

        dataset->write(this, mtype);

        delete dataset;
        delete file;

        return ;
    }

    void fromChars(const std::string& stream)
    {
        (void) stream;

        H5::H5File * file = new H5::H5File(FILE_NAME, H5F_ACC_RDONLY);
        
        H5::StrType CharsType(H5::PredType::C_S1, sizeof(PR::label));
        H5::CompType mtype(sizeof(PR));
        mtype.insertMember("id",     HOFFSET(PR, id),     H5::PredType::NATIVE_INT);
        mtype.insertMember("label",  HOFFSET(PR, label),  CharsType);
        mtype.insertMember("time",   HOFFSET(PR, time),   H5::PredType::NATIVE_INT);
        mtype.insertMember("value",  HOFFSET(PR, value),  H5::PredType::NATIVE_DOUBLE);
        mtype.insertMember("result", HOFFSET(PR, result), H5::PredType::NATIVE_FLOAT);

        H5::DataSet * dataset = new H5::DataSet(file->openDataSet(DATASET_NAME));

        dataset->read(this, mtype);

        delete dataset;
        delete file;

        return ;
    }
};

int main()
{
    PR src;
    
    src.id = 10086;
    memcpy(src.label, "CMCC", 4);
    src.label[4] = '\x0';
    src.time = 12345678;
    src.value = 3.1415926;
    src.result = 0.618;

    std::string nouse;

    src.toChars(nouse);

    // -----
    PR dst;
    
    dst.fromChars(nouse);

    printf("id=%d\n", dst.id);
    printf("label=%s\n", dst.label);
    printf("time=%d\n", dst.time);
    printf("value=%f\n", dst.value);
    printf("result=%f\n", dst.result);

    return 0;
}

