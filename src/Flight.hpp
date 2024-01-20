#ifndef FLIGHT_H
#define FLIGHT_H

#include <vector>
#include <sqlite3.h>
#include <chrono>
#include <ctime>
#include <string>
#include "model.hpp"
#include "DataBaseConnection.hpp"

using namespace std;

class Flight : public Model {
public:
    static Flight* create(time_t departureTime, time_t arrivalTime, string departureAirport, string arrivalAirport, int capacity) {
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "INSERT INTO flights (flightNumber ,departureTime, arrivalTime, departureAirport, arrivalAirport, capacity) VALUES (?, ?, ?, ?, ?, ?)";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        int flightNumber = chrono::system_clock::now().time_since_epoch().count();

        sqlite3_bind_int(stmt, 1, flightNumber);
        sqlite3_bind_int(stmt, 2, departureTime);
        sqlite3_bind_int(stmt, 3, arrivalTime);
        sqlite3_bind_text(stmt, 4, departureAirport.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, arrivalAirport.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, capacity);

        db.executeStatement(stmt);
        db.commit();

        return new Flight(
                   flightNumber,
                   departureTime,
                   arrivalTime,
                   departureAirport,
                   arrivalAirport,
                   capacity
               );
    }

    static vector<Flight> getAllFlights() {
        vector<Flight> flights;

        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "SELECT * FROM flights";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int flightNumber = sqlite3_column_int(stmt, 0);
            time_t departureTime = static_cast<time_t>(sqlite3_column_int(stmt, 1));
            time_t arrivalTime = static_cast<time_t>(sqlite3_column_int(stmt, 2));
            const char* departureAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* arrivalAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            int capacity = sqlite3_column_int(stmt, 5);

            
            //if the number of tickets<capacity
            // if (COUNT<=capacity)
            flights.push_back(
                Flight(
                    flightNumber,
                    departureTime,
                    arrivalTime,
                    departureAirport,
                    arrivalAirport,
                    capacity
                )
            );
        }

        sqlite3_finalize(stmt);

        return flights;
    }


    static Flight getFlightByFlightNumber(int flightNumber) {
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "SELECT * FROM flights WHERE flightNumber = ?";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_int(stmt, 1, flightNumber);

        if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            time_t departureTime = static_cast<time_t>(sqlite3_column_int(stmt, 1));
            time_t arrivalTime = static_cast<time_t>(sqlite3_column_int(stmt, 2));
            const char* departureAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* arrivalAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            int capacity = sqlite3_column_int(stmt, 5);

            Flight flight = Flight(flightNumber, departureTime, arrivalTime, departureAirport, arrivalAirport, capacity);
            sqlite3_finalize(stmt);

            return flight;
        } else
            return Flight(0, 0, 0, "", "", 0);

    }

    static vector<Flight> getFlightByDepartureAirport(string departureAirport) {
        vector<Flight> flights;

        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "SELECT * FROM flights WHERE departureAirport = ?";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_text(stmt, 1, departureAirport.c_str(), -1, SQLITE_STATIC);

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int flightNumber = sqlite3_column_int(stmt, 0);
            time_t departureTime = static_cast<time_t>(sqlite3_column_int(stmt, 1));
            time_t arrivalTime = static_cast<time_t>(sqlite3_column_int(stmt, 2));
            const char* departureAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* arrivalAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            int capacity = sqlite3_column_int(stmt, 5);

            flights.push_back(
                Flight(
                    flightNumber,
                    departureTime,
                    arrivalTime,
                    departureAirport,
                    arrivalAirport,
                    capacity
                )
            );
        }

        sqlite3_finalize(stmt);

        return flights;
    }

    static vector<Flight> getFlightByArrivalAirport(string arrivalAirport) {
        vector<Flight> flights;

        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "SELECT * FROM flights WHERE arrivalAirport = ?";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_text(stmt, 1, arrivalAirport.c_str(), -1, SQLITE_STATIC);

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int flightNumber = sqlite3_column_int(stmt, 0);
            time_t departureTime = static_cast<time_t>(sqlite3_column_int(stmt, 1));
            time_t arrivalTime = static_cast<time_t>(sqlite3_column_int(stmt, 2));
            const char* departureAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* arrivalAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            int capacity = sqlite3_column_int(stmt, 5);

            // SELECT COUNT(*) AS row_count
            // FROM flights;
            //if the number of tickets<capacity
            // if (COUNT<=capacity)
            flights.push_back(
                Flight(
                    flightNumber,
                    departureTime,
                    arrivalTime,
                    departureAirport,
                    arrivalAirport,
                    capacity
                )
            );
        }

        sqlite3_finalize(stmt);

        return flights;
    }

    static void PrintFlights(){
        vector<Flight> availableFlights=getAllFlights();

        cout<<"List of flights: " << endl<<"Flight\t\tDeparture Airport\tDeparture Time\tArrival Time\tArrival Airport"<<endl;
        for(int i=0; i<static_cast<int>(availableFlights.size()); i++)
            cout<<availableFlights[i].flightNumber<<"\t"<<availableFlights[i].departureAirport<<"\t\t\t"<<availableFlights[i].departureTime<<"\t"<<availableFlights[i].arrivalAirport<<"\t\t"<<availableFlights[i].arrivalTime<<endl;
    }
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
    void save() override {
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;
        const char* query = "UPDATE flights SET departureTime = ?, arrivalTime = ?, departureAirport = ?, arrivalAirport = ?, capacity = ? WHERE flightNumber = ?";

        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_int(stmt, 1, departureTime);
        sqlite3_bind_int(stmt, 2, arrivalTime);
        sqlite3_bind_text(stmt, 3, departureAirport.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, arrivalAirport.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, capacity);
        sqlite3_bind_int(stmt, 6, flightNumber);

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE)
            throw runtime_error("Execution failed: " + string(sqlite3_errmsg(db.getDB())));

        // Finalize the statement
        sqlite3_finalize(stmt);

        db.commit();
    }

private:
    int flightNumber;
    time_t departureTime;
    time_t arrivalTime;
    string departureAirport;
    string arrivalAirport;
    int capacity;
    Flight(int flightNumber, time_t departureTime, time_t arrivalTime, string departureAirport, string arrivalAirport, int capacity) {
        this->flightNumber = flightNumber;
        this->departureTime = departureTime;
        this->arrivalTime = arrivalTime;
        this->departureAirport = departureAirport;
        this->arrivalAirport = arrivalAirport;
        this->capacity = capacity;
    }
};

#endif // !FLIGHT_H
