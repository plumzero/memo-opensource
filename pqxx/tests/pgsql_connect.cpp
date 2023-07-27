
#include <iostream>
#include <pqxx/pqxx>

int main()
{
    try {
        pqxx::connection C("dbname=testdb user=postgres password=1234 hostaddr=127.0.0.1 port=5432");
        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        } else {
            std::cout << "Cannot open database" << std::endl;
            return -1;
        }
        C.disconnect();
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