
#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <stdio.h>

int main()
{
  struct Student {
    int id = 0;
    std::string name;
    std::string email;
    float marks = 0;
  };

  auto storage = sqlite_orm::make_storage("db.sqlite",
                              sqlite_orm::make_table("student",
                                          sqlite_orm::make_column("ID", &Student::id, sqlite_orm::primary_key()),
                                          sqlite_orm::make_column("NAME", &Student::name),
                                          sqlite_orm::make_column("EMAIL", &Student::email),
                                          sqlite_orm::make_column("MARKS", &Student::marks)));
  storage.sync_schema();

  storage.replace(Student{1, "Shweta", "shweta@gmail.com", 80});
  storage.replace(Student{2, "Yamini", "rani@gmail.com", 60});

  storage.transaction([&storage] {
    storage.replace(Student{1, "Shweta", "shweta@gmail.com", 80});
    storage.replace(Student{2, "Yamini", "rani@gmail.com", 60});
    storage.replace(Student{3, "Sonal", "sonal@gmail.com", 50});
    storage.replace(Student{4, "Jagruti", "jagu@gmail.com", 30});
    return true;
  });

  //  list all students
  for(auto& student: storage.iterate<Student>()) {
    std::cout << storage.dump(student) << std::endl;
    printf("id=%d,name=%s,email=%s,marks=%.2f\n", student.id, student.name.c_str(), student.email.c_str(), student.marks);
  }
  std::cout << std::endl;

  {  // without alias

    //  SELECT ID, NAME, MARKS,
    //      CASE
    //      WHEN MARKS >=80 THEN 'A+'
    //      WHEN MARKS >=70 THEN 'A'
    //      WHEN MARKS >=60 THEN 'B'
    //      WHEN MARKS >=50 THEN 'C'
    //      ELSE 'Sorry!! Failed'
    //      END
    //      FROM STUDENT;
    auto rows = storage.select(sqlite_orm::columns(&Student::id,
                                        &Student::name,
                                        &Student::marks,
                                        sqlite_orm::case_<std::string>()
                                            .when(sqlite_orm::greater_or_equal(&Student::marks, 80), sqlite_orm::then("A+"))
                                            .when(sqlite_orm::greater_or_equal(&Student::marks, 70), sqlite_orm::then("A"))
                                            .when(sqlite_orm::greater_or_equal(&Student::marks, 60), sqlite_orm::then("B"))
                                            .when(sqlite_orm::greater_or_equal(&Student::marks, 50), sqlite_orm::then("C"))
                                            .else_("Sorry!! Failed")
                                            .end()));
    for(auto& row: rows) {
      std::cout << std::get<0>(row) << ' ' << std::get<1>(row) << ' ' << std::get<2>(row) << ' ' << std::get<3>(row)
           << std::endl;
    }
    std::cout << std::endl;
  }

  return 0;
}
