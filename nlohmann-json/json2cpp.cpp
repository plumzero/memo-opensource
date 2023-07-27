#include <iostream>
#include <vector>
#include <map>
#include "json.hpp"

enum class Gender
{
    THIRD = 0,
    MAN,
    WOMAN
};

class Person
{
public:
    std::string name;
    int age = 18;
    Gender gender = Gender::MAN;
};

class ClassRanking
{
public:
    std::vector<int> rank; // 年纪排名, 班级排名
    std::map<std::string, double> courseGrade; // 科目和成绩
    Person student;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Person, name, age, gender);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClassRanking, rank, courseGrade, student);
NLOHMANN_JSON_SERIALIZE_ENUM(Gender, {{Gender::THIRD, "THIRD"}, 
                                      {Gender::MAN, "MAN"}, {Gender::WOMAN, "WOMAN"}, });

// intrusive
class PersonIntrusive
{
public:
    std::string name;
    int age = 18;
    Gender gender = Gender::MAN;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(PersonIntrusive, name, age, gender);
};

int main (int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    // serialize
    // Person person = {"Jeke", 20, Gender::MAN};
    Person person;
    nlohmann::json j = person;
    std::cout << "dump: " << j.dump() << std::endl;
    
    // unserialize
    std::string buf = "{\"age\":20,\"name\":\"Jeke\", \"gender\":\"MAN\"}";
    nlohmann::json k = nlohmann::json::parse(buf);
    Person fromJson = k;
    std::cout << "name: " << fromJson.name << ", age: " << fromJson.age << ", gender: "
              << int(fromJson.gender) << std::endl;

    ClassRanking rank;
    rank.rank = {10, 2};
    rank.courseGrade = {{"Chinese", 105.5}, {"English", 115.0}, {"Math", 98}};
    rank.student = person;
    nlohmann::json rankJ = rank;
    // std::cout << "rank: " << rankJ.dump(2) << std::endl;
    std::cout << "rank: " << rankJ.dump() << std::endl;

    std::string bufRank = "{\"rank\":[10,2],\"courseGrade\":{\"Chinese\":105.5,\"English\":115.0,\"Math\":98.0},"
                            "\"student\":{\"age\":20,\"name\":\"Jeke\", \"gender\":\"MAN\"}}";
    nlohmann::json rankK = nlohmann::json::parse(bufRank);
    ClassRanking fromJsonRank = rankK;
    std::cout << "course: [" << fromJsonRank.rank.at(0) << "," << fromJsonRank.rank.at(1) << "]" << std::endl;
    std::cout << "courseGrade: ";
    for (auto items : fromJsonRank.courseGrade) {
        std::cout << items.first << ": " << items.second;
    }
    std::cout << std::endl;
    std::cout << "name: " << fromJsonRank.student.name << ", age: "
              << fromJsonRank.student.age << ", gender: " << int(fromJsonRank.student.gender) << std::endl;

    return 0;
}
