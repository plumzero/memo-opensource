
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
  auto storage = sqlite_orm::make_storage("update.sqlite",
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

  storage.replace(User{1, "Ramas",  "California", {0x00, 0x01, 0x00, 0x00}});
  storage.replace(User{2, "Alex",   "Texas", {0x01, 0x02, 0x03, 0x00}});
  storage.replace(User{3, "Alford", "Norway", {0x00, 0x01, 0x00, 0x03}});
  storage.replace(User{4, "Ravi",   "Hall", {0x00, 0x00, 0x00, 0x00}});
  storage.replace(User{5, "Santax", "Texas", {0x01, 0x01, 0x02, 0x02}});

  printf("users count = %d\n", storage.count<User>());

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
    // replace - 替换整条记录
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

  {
    // 不过也提供了根据 id 进行修改的方法，它就是 update
    User user;
    user.id = 2;
    user.name = "Max";
    user.address = "Hond";
    // user.hash = {0x01, 0x02, 0x03, 0x04, 0x05};
    storage.update(user);
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
    // set - 设置一个值
    storage.update_all(sqlite_orm::set(sqlite_orm::assign(&User::hash, std::vector<char>({0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01}))),
                       sqlite_orm::where(sqlite_orm::is_equal(&User::id, 3)));
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
    // set - 设置一个记录中的多个值
    storage.update_all(sqlite_orm::set(sqlite_orm::c(&User::name) = "Teddy",
                       sqlite_orm::c(&User::address) = "Hond",
                       sqlite_orm::c(&User::hash) = std::vector<char>({0x00, 0x01, 0x00})),
                       sqlite_orm::where(sqlite_orm::is_equal(&User::id, 4)));
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
