#ifndef PASSENGER_H
#define PASSENGER_H

#include <sqlite3.h>
#include <chrono>
#include <string>
#include <vector>
#include "model.hpp"
#include <math.h>

using namespace std;

class Passenger : public Model {
public:
    static Passenger* create(string firstName, string lastName, string address, string phoneNumber, string email, int age) {
        // Execute an INSERT query to add a new passenger to the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "INSERT INTO passengers (passportNumber, firstName, lastName, address, phoneNumber, email, age) VALUES (?, ?, ?, ?, ?, ?, ?)";
        long rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        long passportNumber = abs(chrono::system_clock::now().time_since_epoch().count());

        sqlite3_bind_int(stmt, 1, passportNumber);
        sqlite3_bind_text(stmt, 2, firstName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, lastName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, address.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, phoneNumber.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 7, age);

        db.executeStatement(stmt);
        // db.commit();

        return new Passenger(
                   passportNumber,
                   firstName,
                   lastName,
                   address,
                   phoneNumber,
                   email,
                   age
               );
    }

    static vector<Passenger> getAllPassengers() {
        vector<Passenger> passengers;

        // Execute a SELECT query to fetch all passengers from the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "SELECT * FROM passengers";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int passportNumber = sqlite3_column_int(stmt, 0);
            const char* firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const char* address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* phoneNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            int age = sqlite3_column_int(stmt, 6);

            passengers.push_back(
                Passenger(
                    passportNumber,
                    firstName,
                    lastName,
                    address,
                    phoneNumber,
                    email,
                    age
                )
            );
        }

        sqlite3_finalize(stmt);

        return passengers;
    }

    static Passenger getPassengerByPassportNumber(int passportNumber) {

        // Execute a SELECT query to fetch a specific passenger from the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "SELECT * FROM passengers WHERE passportNumber = ?";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_int(stmt, 1, passportNumber);

        if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            const char* firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const char* address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* phoneNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            int age = sqlite3_column_int(stmt, 6);

            Passenger passenger = Passenger(passportNumber, firstName, lastName, address, phoneNumber, email, age);
            sqlite3_finalize(stmt);

            return passenger;
        } else
            return Passenger(0, "", "", "", "", "", 0);
    }

    void save() override {
        // Execute an UPDATE query to modify the passenger information in the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "UPDATE passengers SET firstName = ?, lastName = ?, address = ?, phoneNumber = ?, email = ?, age = ? WHERE passportNumber = ?";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, address.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, phoneNumber.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, age);
        sqlite3_bind_int(stmt, 7, passportNumber);

        db.executeStatement(stmt);
        // db.commit();
    }

    // passportNumber
    int getPassportNumber() {
        return passportNumber;
    }

    // firstName
    string getFirstName() {
        return firstName;
    }
    void setFirstName(string firstName) {
        this->firstName = firstName;
    }

    // lastName
    string getLastName() {
        return lastName;
    }
    void setLastName(string lastName) {
        this->lastName = lastName;
    }

    // address
    string getAddress() {
        return address;
    }
    void setAddress(string address) {
        this->address = address;
    }

    // phoneNumber
    string getPhoneNumber() {
        return phoneNumber;
    }
    void setPhoneNumber(string phoneNumber) {
        this->phoneNumber = phoneNumber;
    }

    // email
    string getEmail() {
        return email;
    }
    void setEmail(string email) {
        this->email = email;
    }

    // age
    int getAge() {
        return age;
    }
    void setAge(int age) {
        this->age = age;
    }

private:
    int passportNumber;
    string firstName;
    string lastName;
    string address;
    string phoneNumber;
    string email;
    int age;

    Passenger (int passportNumber, string firstName, string lastName, string address, string phoneNumber, string email, int age) {
        this->passportNumber = passportNumber;
        this->firstName = firstName;
        this->lastName = lastName;
        this->address = address;
        this->phoneNumber = phoneNumber;
        this->email = email;
        this->age = age;
    }

};

#endif // !PASSENGER_H