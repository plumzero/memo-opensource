
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

        sql = "SELECT * FROM enterprise";

        pqxx::nontransaction ntxn(conn);

        pqxx::result sqlres = ntxn.exec(sql);

        std::cout << "Found " << sqlres.size() << " employees:" << std::endl;

        ntxn.commit();
        for (pqxx::result::const_iterator it = sqlres.begin(); it != sqlres.end(); it++) {
            std::cout << std::setiosflags(std::ios::left);
            std::cout << "ID = " << std::setw(10) << it[0].as<int>();
            std::cout << "Name = " << std::setw(10) << it[1].as<std::string>();
            std::cout << "Age = " << std::setw(10) << it[2].as<int>();
            std::cout << "Address = " << std::setw(10) << it[3].as<std::string>();
            std::cout << "Salary = " << std::setw(10) << it[4].as<float>() << std::endl;
        }

        /** 第 2 种访问方式 */
        // const int num_rows = sqlres.size();
        // for (int rownum=0; rownum < num_rows; ++rownum)
        // {
        //     const pqxx::row row_ = sqlres[rownum];
        //     const int num_cols = row_.size();
        //     for (int colnum=0; colnum < num_cols; ++colnum)
        //     {
        //         const pqxx::field field = row_[colnum];
        //         std::cout << field.c_str() << '\t';
        //     }

        //     std::cout << std::endl;
        // }

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