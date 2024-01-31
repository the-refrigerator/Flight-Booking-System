#include <iostream>
#include <sqlite3.h>
#include <stdexcept>
#include <fstream>
#include "../headers/DataBaseConnection.h"

using namespace std;

DataBaseConnection& DataBaseConnection::getInstance() {
    static DataBaseConnection connection;
    return connection;
}

void DataBaseConnection::executeStatement (sqlite3_stmt* stmt) {
    int rc = sqlite3_exec(getDB(), "BEGIN TRANSACTION", 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot begin transaction: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    commit();
}

void DataBaseConnection::commit() {
    // Commit the transaction
    int rc = sqlite3_exec(db, "COMMIT", 0, 0, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot commit transaction: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

}

sqlite3* DataBaseConnection::getDB() {
    return db;
}


DataBaseConnection::DataBaseConnection() {
    std::ifstream file("database.db");

    if (!file.good()) {
        std::ofstream outfile("database.db");
        outfile.close();
    }

    sqlite3* db;
    int rc = sqlite3_open("database.db", &db);

    if (rc) {
        cerr << "Error connecting to database: " << sqlite3_errmsg(db) << endl;
        throw runtime_error("Error connecting to database");
    }

    this->db = db;

    createTables();
}

void DataBaseConnection::createTables() {
    const char* createFlightsTableQuery =
        "CREATE TABLE IF NOT EXISTS flights ("
        "    flightNumber INTEGER PRIMARY KEY,"
        "    departureTime TEXT,"
        "    arrivalTime TEXT,"
        "    departureAirport TEXT,"
        "    arrivalAirport TEXT,"
        "    capacity INTEGER"
        ");";

    const char* createPassengersTableQuery =
        "CREATE TABLE IF NOT EXISTS passengers ("
        "    passportNumber INTEGER PRIMARY KEY,"
        "    firstName TEXT,"
        "    lastName TEXT,"
        "    address TEXT,"
        "    phoneNumber TEXT UNIQUE,"
        "    email TEXT UNIQUE,"
        "    age INTEGER"
        ");";

    const char* createTicketsTableQuery =
        "CREATE TABLE IF NOT EXISTS tickets ("
        "    ticketNumber INTEGER PRIMARY KEY,"
        "    flightNumber INTEGER,"
        "    passportNumber INTEGER,"
        "    seatNumber TEXT,"
        "    price REAL,"
        "    FOREIGN KEY (flightNumber) REFERENCES flights(flightNumber) ON DELETE CASCADE,"
        "    FOREIGN KEY (passportNumber) REFERENCES passengers(passportNumber) ON DELETE CASCADE"
        ");";

    const char* createDirectorsTableQuery =
        "CREATE TABLE IF NOT EXISTS directors ("
        "    idNumber INTEGER PRIMARY KEY,"
        "    firstName TEXT,"
        "    lastName TEXT,"
        "    address TEXT,"
        "    phoneNumber TEXT,"
        "    email TEXT,"
        "    age INTEGER"
        ");";


    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(getDB(), createFlightsTableQuery, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot prepare statement (" << createFlightsTableQuery << "): " << sqlite3_errmsg(getDB()) << std::endl;
        return;
    }

    executeStatement(stmt);

    rc = sqlite3_prepare_v2(getDB(), createPassengersTableQuery, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot prepare statement (" << createPassengersTableQuery << "): " << sqlite3_errmsg(getDB()) << std::endl;
        return;
    }

    executeStatement(stmt);

    rc = sqlite3_prepare_v2(getDB(), createTicketsTableQuery, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot prepare statement (" << createTicketsTableQuery << "): " << sqlite3_errmsg(getDB()) << std::endl;
        return;
    }

    executeStatement(stmt);


    rc = sqlite3_prepare_v2(getDB(), createDirectorsTableQuery, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot prepare statement (" << createDirectorsTableQuery << "): " << sqlite3_errmsg(getDB()) << std::endl;
        return;
    }

    executeStatement(stmt);
}
