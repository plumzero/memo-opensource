
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
  auto storage = sqlite_orm::make_storage("insert.sqlite",
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

  {
    // insert
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
  }

  {
    // insert 插入会按 id 依次插入，所以像下面虽然也能插入，但是实际插入的 id 却是 5
    storage.insert(User{1, "Santax", "Texas", {0x01, 0x01, 0x02, 0x02}});
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
    // 如果需要以特定 ID 插入新用户，可以使用 replace
    storage.replace(User{1, "Paul", "Houston", {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09}});
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

  return 0;
}
