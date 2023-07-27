
#include <iostream>
#include <pqxx/pqxx>

int main()
{
    const char* sql = NULL;
    
    try {
        pqxx::connection conn("dbname=testdb user=postgres password=1234 hostaddr=127.0.0.1 port=5432");
        if (conn.is_open()) {
            std::cout << "Opened database successfully: " << conn.dbname() << std::endl;
        } else {
            std::cout << "Cannot open database" << std::endl;
            return -1;
        }
        
        sql = "CREATE TABLE enterprise("            \
              "id       INT PRIMARY KEY NOT NULL,"  \
              "name     TEXT            NOT NULL,"  \
              "age      INT             NOT NULL,"  \
              "address  CHAR(50),"                  \
              "salary   REAL );";
        
        pqxx::work txn(conn);
        txn.exec(sql);
        txn.commit();
        std::cout << "Table created successfully" << std::endl;
        conn.disconnect();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << e.query() << std::endl;
        return -1;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}