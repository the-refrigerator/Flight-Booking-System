#include <vector>
#include <sqlite3.h>
#include <chrono>
#include <string>
#include "../headers/DataBaseConnection.h"
#include "../headers/Flight.h"

using namespace std;

Flight* Flight::create(string departureTime, string arrivalTime, string departureAirport, string arrivalAirport, int capacity) {
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "INSERT INTO flights (flightNumber ,departureTime, arrivalTime, departureAirport, arrivalAirport, capacity) VALUES (?, ?, ?, ?, ?, ?)";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    int flightNumber = static_cast<int>(abs(chrono::system_clock::now().time_since_epoch().count()));

    sqlite3_bind_int(stmt, 1, flightNumber);
    sqlite3_bind_text(stmt, 2, departureTime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, arrivalTime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, departureAirport.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, arrivalAirport.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, capacity);

    db.executeStatement(stmt);

    return new Flight(
               flightNumber,
               departureTime,
               arrivalTime,
               departureAirport,
               arrivalAirport,
               capacity
           );
}

vector<Flight> Flight::getAllFlights() {
    vector<Flight> flights;

    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "SELECT * FROM flights";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int flightNumber = sqlite3_column_int(stmt, 0);
        const char* departureTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* arrivalTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
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


Flight Flight::getFlightByFlightNumber(int flightNumber) {
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "SELECT * FROM flights WHERE flightNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    sqlite3_bind_int(stmt, 1, flightNumber);

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int flightNumber = sqlite3_column_int(stmt, 0);
        const char* departureTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* arrivalTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* departureAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* arrivalAirport = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        int capacity = sqlite3_column_int(stmt, 5);

        Flight flight = Flight(flightNumber, departureTime, arrivalTime, departureAirport, arrivalAirport, capacity);
        sqlite3_finalize(stmt);

        return flight;
    } else
        return Flight(0, 0, 0, "", "", 0);

}

vector<Flight> Flight::getFlightByDepartureAirport(string departureAirport) {
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
        const char* departureTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* arrivalTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
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

vector<Flight> Flight::getFlightByArrivalAirport(string arrivalAirport) {
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
        const char* departureTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* arrivalTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
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

void Flight::PrintFlights() {
    vector<Flight> availableFlights = getAllFlights();

    cout << "List of flights: " << endl << "Flight\tFlight Number\t\tDeparture Airport\tDeparture Time\tArrival Airport\tArrival Time\tAvailable Tickets" << endl;

    for(int i = 0; i < static_cast<int>(availableFlights.size()); i++)
        cout << i + 1 << "\t" << availableFlights[i].flightNumber << "\t\t" << availableFlights[i].departureAirport << "\t\t\t" << availableFlights[i].departureTime << "\t" << availableFlights[i].arrivalAirport << "\t\t" << availableFlights[i].arrivalTime << "\t" << availableFlights[i].capacity - availableFlights[i].numberOFTickets << endl;
}

void Flight::PrintFlightsVector(vector<Flight> flights) {
    cout << "List of flights: " << endl << "Flight\tFlight Number\t\tDeparture Airport\tDeparture Time\tArrival Airport\tArrival Time\tAvailable Tickets" << endl;

    for(int i = 0; i < static_cast<int>(flights.size()); i++)
        cout << i + 1 << "\t" << flights[i].flightNumber << "\t\t" << flights[i].departureAirport << "\t\t\t" << flights[i].departureTime << "\t" << flights[i].arrivalAirport << "\t\t" << flights[i].arrivalTime << "\t" << flights[i].capacity - flights[i].numberOFTickets << endl;
}

void Flight::PrintFlight() {
    cout << "Flight Number: " << flightNumber << endl;
    cout << "Departure Airport: " << departureAirport << endl;
    cout << "Departure Time: " << departureTime << endl;
    cout << "Arrival Airport: " << arrivalAirport << endl;
    cout << "Arrival Time: " << arrivalTime << endl;
    cout << "Available Tickets: " << capacity - numberOFTickets << endl;
}

// flightNumber
int Flight::getFlightNumber() {
    return flightNumber;
}

// departureTime
string Flight::getDepartureTime() {
    return departureTime;
}
void Flight::setDepartureTime(string departureTime) {
    this->departureTime = departureTime;
}

// arrivalTime
string Flight::getArrivalTime() {
    return arrivalTime;
}
void Flight::setArrivalTime(string arrivalTime) {
    this->arrivalTime = arrivalTime;
}

// departureAirport
string Flight::getDepartureAirport() {
    return departureAirport;
}
void Flight::setDepartureAirport(string departureAirport) {
    this->departureAirport = departureAirport;
}

// arrivalAirport
string Flight::getArrivalAirport() {
    return arrivalAirport;
}
void Flight::setArrivalAirport(string arrivalAirport) {
    this->arrivalAirport = arrivalAirport;
}

// capacity
int Flight::getCapacity() {
    return capacity;
}

//Number of Tickets
int Flight::getNumberOfTickets() {
    return numberOFTickets;
}

void Flight::incrementTickets() {
    numberOFTickets++;
}

void Flight::decrementTickets() {
    numberOFTickets--;
}

void Flight::remove() {
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "DELETE FROM flights WHERE flightNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    sqlite3_bind_int(stmt, 1, flightNumber);

    db.executeStatement(stmt);
}

void Flight::save() {
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;
    const char* query = "UPDATE flights SET departureTime = ?, arrivalTime = ?, departureAirport = ?, arrivalAirport = ?, capacity = ? WHERE flightNumber = ?";

    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    sqlite3_bind_text(stmt, 1, departureTime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, arrivalTime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, departureAirport.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, arrivalAirport.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, capacity);
    sqlite3_bind_int(stmt, 6, flightNumber);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
        throw runtime_error("Execution failed: " + string(sqlite3_errmsg(db.getDB())));

    // Finalize the statement
    sqlite3_finalize(stmt);
}

Flight::Flight(int flightNumber, string departureTime, string arrivalTime, string departureAirport, string arrivalAirport, int capacity) {
    this->flightNumber = flightNumber;
    this->departureTime = departureTime;
    this->arrivalTime = arrivalTime;
    this->departureAirport = departureAirport;
    this->arrivalAirport = arrivalAirport;
    this->capacity = capacity;
}
