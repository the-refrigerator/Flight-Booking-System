#pragma once
#include <iostream>
#include <sqlite3.h>
#include <chrono>
#include <ctime>
#include <string>
#include <system_error>
#include <vector>

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


class Flight {
public:
    Flight (time_t departureTime, time_t arrivalTime, string departureAirport, string arrivalAirport, int capacity) {
        this->flightNumber = chrono::system_clock::now().time_since_epoch().count();
        this->departureTime = departureTime;
        this->arrivalTime = arrivalTime;
        this->departureAirport = departureAirport;
        this->arrivalAirport = arrivalAirport;
        this->capacity = capacity;
    }

    // static vector<Flight> getAllFlights() {
    // TODO: implement getAllFlights
    // }

    // static Flight getFlightByFlightNumber(int flightNumber) {
    // TODO: implement getFlightByFlightNumber
    // }

    // static vector<Flight> getFlightByDepartureAirport(string departureAirport) {
    // TODO: implement getFlightByDepartureAirport
    // }

    // static vector<Flight> getFlightByArrivalAirport(string arrivalAirport) {
    // TODO: implement getFlightByArrivalAirport
    // }

    // static vector<Flight> getFlightByDepartureAirport(string departureAirport) {
    // TODO: implement getFlightByDepartureAirport
    // }

    // flightNumber
    int getFlightNumber() {
        return flightNumber;
    }

    // departureTime
    time_t getDepartureTime() {
        return departureTime;
    }
    void setDepartureTime(time_t departureTime) {
        this->departureTime = departureTime;
    }

    // arrivalTime
    time_t getArrivalTime() {
        return arrivalTime;
    }
    void setArrivalTime(time_t arrivalTime) {
        this->arrivalTime = arrivalTime;
    }

    // departureAirport
    string getDepartureAirport() {
        return departureAirport;
    }
    void setDepartureAirport(string departureAirport) {
        this->departureAirport = departureAirport;
    }

    // arrivalAirport
    string getArrivalAirport() {
        return arrivalAirport;
    }
    void setArrivalAirport(string arrivalAirport) {
        this->arrivalAirport = arrivalAirport;
    }

    // capacity
    int getCapacity() {
        return capacity;
    }

    // Save to database
    void save() {
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db.getDB(), "WRITE THE UPDATE STATEMENT HERE", -1, &stmt, 0);

        if (rc != SQLITE_OK) {
            std::cerr << "Cannot prepare statement: " << sqlite3_errmsg(db.getDB()) << std::endl;
            return;
        }


        db.executeStatement(stmt);
    }

private:
    int flightNumber;
    time_t departureTime;
    time_t arrivalTime;
    string departureAirport;
    string arrivalAirport;
    int capacity;
};

class Passenger {
public:
    Passenger (string firstName, string lastName, string address, string phoneNumber, string email, int age) {
        this->passportNumber = chrono::system_clock::now().time_since_epoch().count();
        this->firstName = firstName;
        this->lastName = lastName;
        this->address = address;
        this->phoneNumber = phoneNumber;
        this->email = email;
        this->age = age;

        // TODO: add to database
    }
    // static vector<Passenger> getAllPassengers() {
    //     // TODO: implement getAllPassengers
    //     return vector<Passenger>();
    // }

    // static Passenger getPassengerByPassportNumber(int passportNumber) {
    //     // TODO: implement getPassengerByPassportNumber
    //     // return Passenger();
    // }

    // static vector<Passenger> getPassengerByFirstName(string Name) {
    //      // TODO: implement getPassengerByFirstName
    // }

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

class Ticket {
public:
    Ticket (int flightNumber, int passportNumber, string seatNumber, double price) {
        this->ticketNumber = chrono::system_clock::now().time_since_epoch().count();
        this->flightNumber = flightNumber;
        this->passportNumber = passportNumber;
        this->seatNumber = seatNumber;
        this->price = price;
    }

    // static vector<Ticket> getAllTicketsOfFlight(int flightNumber) {
    //  TODO: implement getAllTicketsOfFlight
    //}

    // static vector<Ticket> getAllTicketsOfPassenger(int passportNumber){
    // TODO: implement getAllTicketsOfPassenger
    // }

private:
    int ticketNumber;
    int flightNumber;
    int passportNumber;
    string seatNumber;
    double price;
};

