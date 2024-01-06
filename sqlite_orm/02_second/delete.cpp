
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdint.h>

struct User {
  int id;
  std::string name;
  std::string address;
  std::vector<char> hash; // binary format
};

int main(int, char **)
{
  auto storage = sqlite_orm::make_storage("delete.sqlite",
                              sqlite_orm::make_table("users",
                                          sqlite_orm::make_column("id", &User::id, sqlite_orm::primary_key()),
                                          sqlite_orm::make_column("name", &User::name),
                                          sqlite_orm::make_column("address", &User::address),
                                          sqlite_orm::make_column("hash", &User::hash)));
  
  storage.sync_schema();
  if (! storage.table_exists("users")) {
    printf("cannot find table 'users'\n");
    return -1;
  }

  storage.remove_all<User>();

  storage.insert(User{1, "Ramas",  "California", {0x00, 0x01, 0x00, 0x00}});
  storage.insert(User{2, "Alex",   "Texas", {0x01, 0x02, 0x03, 0x00}});
  storage.insert(User{3, "Alford", "Norway", {0x00, 0x01, 0x00, 0x03}});
  storage.insert(User{4, "Ravi",   "Hall", {0x00, 0x00, 0x00, 0x00}});

  auto rows = storage.select(sqlite_orm::object<User>());
  printf("rows.size=%ld\n", rows.size());
  printf("id\tname\taddress\thash\n");
  for (auto &row : rows) {
    printf("%d\t%s\t%s\t", row.id, row.name.c_str(), row.address.c_str());
    std::vector<char> bytes = row.hash;
    printf("(%ld)", bytes.size());
    for (size_t i = 0; i < bytes.size(); i++) {
      printf("%02X", (int)static_cast<uint8_t>(bytes[i]));
    }
    printf("\n");
  }
  printf("\n");


  {
    // remove: 通过 id 来删除。因为 delete 是 C++ 关键字，所以使用 remove 来代替
    int id = 3;
    storage.remove<User>(id);
    auto rows = storage.select(sqlite_orm::object<User>());
    printf("rows.size=%ld\n", rows.size());
    printf("id\tname\taddress\thash\n");
    for (auto &row : rows) {
      printf("%d\t%s\t%s\t", row.id, row.name.c_str(), row.address.c_str());
      std::vector<char> bytes = row.hash;
      printf("(%ld)", bytes.size());
      for (size_t i = 0; i < bytes.size(); i++) {
        printf("%02X", (int)static_cast<uint8_t>(bytes[i]));
      }
      printf("\n");
    }
    printf("\n");
  }

  {
    // 删除所有记录
    storage.remove_all<User>();
    auto rows = storage.select(sqlite_orm::object<User>());
    printf("rows.size=%ld\n", rows.size());
    printf("id\tname\taddress\thash\n");
    for (auto &row : rows) {
      printf("%d\t%s\t%s\t", row.id, row.name.c_str(), row.address.c_str());
      std::vector<char> bytes = row.hash;
      printf("(%ld)", bytes.size());
      for (size_t i = 0; i < bytes.size(); i++) {
        printf("%02X", (int)static_cast<uint8_t>(bytes[i]));
      }
      printf("\n");
    }
    printf("\n");

    if (! storage.table_exists("users")) {
      printf("cannot find table 'users'\n");
      return -1;
    }
  }

  {
    if (storage.table_exists("users")) {
      storage.drop_table("users");
      printf("'users' been dropped success\n");
    }
    // storage.drop_table("users");  // 删除了就不能再删除了
  }

  {
    // 删除数据库
    ::remove("delete.sqlite");
    printf("'sqlite' been dropped success\n");
  }

  return 0;
}
