
#include <iostream>
#include <iomanip>
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
        
        sql = "DELETE FROM enterprise WHERE id = 2";
        pqxx::work txn(conn);
        txn.exec(sql);
        txn.commit();
        std::cout << "Records deleted successfully" << std::endl;
        
        sql = "SELECT * FROM enterprise";
        pqxx::nontransaction ntxn(conn);
        pqxx::result sqlres = ntxn.exec(sql);
        for (pqxx::result::const_iterator it = sqlres.begin(); it != sqlres.end(); it++) {
            std::cout << std::setiosflags(std::ios::left);
            std::cout << "ID = " << std::setw(10) << it[0].as<int>();
            std::cout << "Name = " << std::setw(10) << it[1].as<std::string>();
            std::cout << "Age = " << std::setw(10) << it[2].as<int>();
            std::cout << "Address = " << std::setw(10) << it[3].as<std::string>();
            std::cout << "Salary = " << std::setw(10) << it[4].as<float>() << std::endl;
        }
        std::cout << "Operation done successfully" << std::endl;
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