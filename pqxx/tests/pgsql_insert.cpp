
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
        
        sql = "INSERT INTO enterprise(id, name, age, address, salary)"      \
              "VALUES"                                                      \
              "(1, 'Paul', 32, 'California', 20000.00),"                    \
              "(2, 'Allen', 25, 'Texas', 15000.00),"                        \
              "(3, 'Teddy', 23, 'Norway', 20000.00),"                       \
              "(4, 'Mark', 25, 'Rich-Mond', 65000.00);";
        
        pqxx::work txn(conn);
        txn.exec(sql);
        txn.commit();
        std::cout << "Records created successfully" << std::endl;
        conn.disconnect();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << e.query() << std::endl;
        return -1;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}