#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <sqlite3.h>

class DataBaseConnection {
    // Follows a singleton pattern
public:
    static DataBaseConnection& getInstance();

    void executeStatement (sqlite3_stmt* stmt);

    void commit();

    sqlite3* getDB();

private:
    sqlite3* db;

    DataBaseConnection();

    void createTables();
};

#endif // !DATABASECONNECTION_H

