#ifndef TICKET_H
#define TICKET_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include "model.h"

using namespace std;

class Ticket : public Model {
public:
    static Ticket* create(int flightNumber, int passportNumber, string seatNumber, double price);
    static vector<Ticket> getAllTicketsOfFlight(int flightNumber);
    static vector<Ticket> getAllTicketsOfPassenger(int passportNumber);
    void remove();
    void save() override;

private:
    int ticketNumber;
    int flightNumber;
    int passportNumber;
    string seatNumber;
    double price;
    Ticket (int ticketNumber, int flightNumber, int passportNumber, string seatNumber, double price);
};

#endif // !TICKET_H
