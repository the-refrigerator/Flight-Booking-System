#include <sqlite3.h>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include "../headers/DataBaseConnection.h"
#include "../headers/Ticket.h"

using namespace std;

Ticket* Ticket::create(int flightNumber, int passportNumber, string seatNumber, double price) {
    // Execute an INSERT query to add a new ticket to the database
    // Use the database connection from DataBaseConnection singleton
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "INSERT INTO tickets (ticketNumber, flightNumber, passportNumber, seatNumber, price) VALUES (?, ?, ?, ?, ?)";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    int ticketNumber = abs(static_cast<int>(abs(chrono::system_clock::now().time_since_epoch().count())));

    sqlite3_bind_int(stmt, 1, ticketNumber);
    sqlite3_bind_int(stmt, 2, flightNumber);
    sqlite3_bind_int(stmt, 3, passportNumber);
    sqlite3_bind_text(stmt, 4, seatNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 5, price);

    db.executeStatement(stmt);

    return new Ticket(
               ticketNumber,
               flightNumber,
               passportNumber,
               seatNumber,
               price
           );
}

Ticket* Ticket::getTicketByTicketNumber(int ticketNumber) {
    // Execute a SELECT query to fetch a ticket from the database
    // Use the database connection from DataBaseConnection singleton
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "SELECT * FROM tickets WHERE ticketNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        cerr << "Cannot prepare statement: " << sqlite3_errmsg(db.getDB()) << endl;
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, ticketNumber);

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int ticketNumber = sqlite3_column_int(stmt, 0);
        int flightNumber = sqlite3_column_int(stmt, 1);
        int passportNumber = sqlite3_column_int(stmt, 2);
        const char* seatNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        double price = sqlite3_column_double(stmt, 4);

        return new Ticket(
                   ticketNumber,
                   flightNumber,
                   passportNumber,
                   seatNumber,
                   price
               );
    }

    sqlite3_finalize(stmt);

    return new Ticket(0, 0, 0, "", 0);
}

vector<Ticket> Ticket::getAllTicketsOfFlight(int flightNumber) {
    vector<Ticket> tickets;

    // Execute a SELECT query to fetch all tickets of a specific flight from the database
    // Use the database connection from DataBaseConnection singleton
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "SELECT * FROM tickets WHERE flightNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        cerr << "Cannot prepare statement: " << sqlite3_errmsg(db.getDB()) << endl;
        return tickets;
    }

    sqlite3_bind_int(stmt, 1, flightNumber);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int ticketNumber = sqlite3_column_int(stmt, 0);
        int flightNumber = sqlite3_column_int(stmt, 1);
        int passportNumber = sqlite3_column_int(stmt, 2);
        const char* seatNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        double price = sqlite3_column_double(stmt, 4);

        tickets.push_back(
            Ticket(
                ticketNumber,
                flightNumber,
                passportNumber,
                seatNumber,
                price
            )
        );
    }

    sqlite3_finalize(stmt);

    return tickets;

}

vector<Ticket> Ticket::getAllTicketsOfPassenger(int passportNumber) {
    vector<Ticket> tickets;

    // Execute a SELECT query to fetch all tickets of a specific passenger from the database
    // Use the database connection from DataBaseConnection singleton
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "SELECT * FROM tickets WHERE passportNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        cerr << "Cannot prepare statement: " << sqlite3_errmsg(db.getDB()) << endl;
        return tickets;
    }

    sqlite3_bind_int(stmt, 1, passportNumber);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int ticketNumber = sqlite3_column_int(stmt, 0);
        int flightNumber = sqlite3_column_int(stmt, 1);
        int passportNumber = sqlite3_column_int(stmt, 2);
        const char* seatNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        double price = sqlite3_column_double(stmt, 4);

        tickets.push_back(
            Ticket(
                ticketNumber,
                flightNumber,
                passportNumber,
                seatNumber,
                price
            )
        );
    }

    sqlite3_finalize(stmt);

    return tickets;
}

void Ticket::remove() {
    // Execute a DELETE query to remove the ticket from the database
    // Use the database connection from DataBaseConnection singleton
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "DELETE FROM tickets WHERE ticketNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        cerr << "Cannot prepare statement: " << sqlite3_errmsg(db.getDB()) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, ticketNumber);

    db.executeStatement(stmt);
}

void Ticket::save() {
    // Execute an UPDATE query to modify the ticket information in the database
    // Use the database connection from DataBaseConnection singleton
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "UPDATE tickets SET flightNumber = ?, passportNumber = ?, seatNumber = ?, price = ? WHERE ticketNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        cerr << "Cannot prepare statement: " << sqlite3_errmsg(db.getDB()) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, flightNumber);
    sqlite3_bind_int(stmt, 2, passportNumber);
    sqlite3_bind_text(stmt, 3, seatNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, price);
    sqlite3_bind_int(stmt, 5, ticketNumber);

    db.executeStatement(stmt);
}

int Ticket::getTicketNumber() {
    return ticketNumber;
}

int Ticket::getFlightNumber() {
    return flightNumber;
}

int Ticket::getPassportNumber() {
    return passportNumber;
}

string Ticket::getSeatNumber() {
    return seatNumber;
}

double Ticket::getPrice() {
    return price;
}

Ticket::Ticket (int ticketNumber, int flightNumber, int passportNumber, string seatNumber, double price) {
    this->ticketNumber = ticketNumber;
    this->flightNumber = flightNumber;
    this->passportNumber = passportNumber;
    this->seatNumber = seatNumber;
    this->price = price;
}
