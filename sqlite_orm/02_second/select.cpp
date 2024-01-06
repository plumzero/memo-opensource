
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <memory>
#include <iostream>
#include <thread>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>

struct User {
  int id;
  std::string name;
  double score;
  std::vector<char> binaryCode;
};

std::vector<char> random8(void)
{
  srand(time(NULL));

  uint64_t val;
  val = (uint64_t)lrand48();
  val <<= 32;
  val += (uint64_t)lrand48();
  std::vector<char> vec(8);
  memcpy(vec.data(), &val, 8);
  for (size_t i = 0; i < vec.size(); i++) {
    printf("%02X", (int)static_cast<uint8_t>(vec[i]));
  }
  printf("\n");
  return vec;
}

int main()
{
  // 创建存储
  auto storage = sqlite_orm::make_storage("select.sqlite",
                              sqlite_orm::make_table("users",
                                        sqlite_orm::make_column("id", &User::id, sqlite_orm::autoincrement(), sqlite_orm::primary_key()),
                                        sqlite_orm::make_column("name", &User::name),
                                        sqlite_orm::make_column("score", &User::score),
                                        sqlite_orm::make_column("binary_code", &User::binaryCode)));
  // 同步到文件系统
  storage.sync_schema();
  // 检查
  std::cout << "storage.filename: " << storage.filename() << std::endl;
  std::cout << std::boolalpha << storage.table_exists("users") << std::endl;
  // 插入数据
  storage.replace(User{1, "Ramas", 0.15, random8()});
  storage.replace(User{2, "Alex", 0.13, random8()});
  storage.replace(User{3, "Alford", 0.12, random8()});
  storage.replace(User{4, "Ravi", 0.15, random8()});
  storage.replace(User{5, "Santax", 0.14, random8()});
  
  {
    // dump 查看
    //  list all students
    for(auto& user: storage.iterate<User>()) {
      std::cout << storage.dump(user).c_str() << std::endl;
      printf("id=%d,name=%s,score=%.2f\n", user.id, user.name.c_str(), user.score);
    }
    std::cout << std::endl;
  }

  {
    // 查询数据 - 将查询结果保存为 std::tuple。相当于 SELECT id,name,score FROM users;
    auto rows = storage.select(sqlite_orm::columns(&User::id, &User::name, &User::score, &User::binaryCode));
    // decltype(rows) is std::vector<std::tuple<...all column typed in declared in make_table order...>>
    printf("id\tname\tscore\tbinary_code\n");
    for(auto &row: rows) {
      printf("%d\t%s\t%.2f\t", std::get<0>(row), std::get<1>(row).c_str(), std::get<2>(row));
      std::vector<char> bytes = std::get<3>(row);
      printf("(%ld)", bytes.size());
      for (size_t i = 0; i < bytes.size(); i++) {
        printf("%02X", (int)static_cast<uint8_t>(bytes[i]));
      }
      printf("\n");
    }
    printf("\n");
  }

  {
    // 查询数据 - 将查询结果保存为 std::vector<User>。相当于 SELECT * FROM users;
    auto rows = storage.select(sqlite_orm::object<User>());
    // decltype(rows) is std::vector<User>
    printf("rows.size=%ld\n", rows.size());
    printf("id\tname\tscore\tbinary_code\n");
    for (auto &row : rows) {
      printf("%d\t%s\t%.2f\t", row.id, row.name.c_str(), row.score);
      std::vector<char> bytes = row.binaryCode;
      printf("(%ld)", bytes.size());
      for (size_t i = 0; i < bytes.size(); i++) {
        printf("%02X", (int)static_cast<uint8_t>(bytes[i]));
      }
      printf("\n");
    }
    printf("\n");
  }

  {
    // 查询 - 通过 id 查询
    // 如果查询的 id 不存在，这时程序会抛出异常。开发者可以使用 get_pointer 返回一个 unique_ptr 来避免抛出异常，当 id 不存在时，返回空
    int id = 4;
    if (auto user = storage.get_pointer<User>(id)){
      std::cout << "user = " << user->name << " " << user->score << std::endl;
    } else {
      std::cout << "no user with id " << id << std::endl;
    }
  }

  {
    // 条件查询 - 示例一
    auto w = sqlite_orm::where(sqlite_orm::is_equal(&User::id, 3));
    auto rows = storage.select(sqlite_orm::object<User>(), w);
    printf("rows.size=%ld\n", rows.size());
    printf("id\tname\tscore\tbinary_code\n");
    for (auto &row : rows) {
      printf("%d\t%s\t%.2f\t", row.id, row.name.c_str(), row.score);
      std::vector<char> bytes = row.binaryCode;
      printf("(%ld)", bytes.size());
      for (size_t i = 0; i < bytes.size(); i++) {
        printf("%02X", (int)static_cast<uint8_t>(bytes[i]));
      }
      printf("\n");
    }
    printf("\n");
  }

  {
    // 条件查询 - 示例二
    auto w = sqlite_orm::where(sqlite_orm::is_equal(&User::name, "Ravi"));
    auto rows = storage.select(sqlite_orm::object<User>(), w);
    printf("rows.size=%ld\n", rows.size());
    printf("id\tname\tscore\tbinary_code\n");
    for (auto &row : rows) {
      printf("%d\t%s\t%.2f\t", row.id, row.name.c_str(), row.score);
      std::vector<char> bytes = row.binaryCode;
      printf("(%ld)", bytes.size());
      for (size_t i = 0; i < bytes.size(); i++) {
        printf("%02X", (int)static_cast<uint8_t>(bytes[i]));
      }
      printf("\n");
    }
    printf("\n");
  }

  return 0;
}