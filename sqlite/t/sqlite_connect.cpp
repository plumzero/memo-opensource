
#include <stdio.h>
#include <sqlite3.h>

/**
 * 连接一个数据库。如果数据库不存在，那么它就会被创建，最后将返回一个数据库对象。
 * 
 * 静态依赖编译:
 *  g++ -g -std=c++11 sqlite_connect.cpp -I/usr/local/sqlite/include /usr/local/sqlite/lib/libsqlite3.a \
 *      -o sqlite_connect -lpthread -ldl -lm
 */

int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);

    return 0;
}