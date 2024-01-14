#ifndef MDOELS_H
#define MDOELS_H

#include <iostream>
#include <sqlite3.h>

using namespace std;

class DataBaseConnection {
    // Follows a singleton pattern
public:
    static DataBaseConnection& getInstance() {
        static DataBaseConnection connection;
        return connection;
    }

    void executeStatement (sqlite3_stmt* stmt) {
        int rc = sqlite3_exec(this->db, "BEGIN TRANSACTION", 0, 0, 0);

        if (rc != SQLITE_OK)
            std::cerr << "Cannot begin transaction: " << sqlite3_errmsg(db) << std::endl;

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Finalize the statement
        sqlite3_finalize(stmt);

    }

    void commit() {
        // Commit the transaction
        int rc = sqlite3_exec(db, "COMMIT", 0, 0, 0);

        if (rc != SQLITE_OK) {
            std::cerr << "Cannot commit transaction: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

    }

    sqlite3* getDB() {
        return db;
    }

private:
    sqlite3* db;

    DataBaseConnection() {
        sqlite3* db;
        int rc = sqlite3_open("database.db", &db);

        if (rc) {
            cerr << "Error connecting to database: " << sqlite3_errmsg(db) << endl;
            return;
        }

        this->db = db;
    }
};

#endif // !MDOELS_H
