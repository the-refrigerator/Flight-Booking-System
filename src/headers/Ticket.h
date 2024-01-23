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
    static Ticket* getTicketByTicketNumber(int ticketNumber);
    void remove();
    void save() override;
    int getTicketNumber();
    int getFlightNumber();
    int getPassportNumber();
    string getSeatNumber();
    double getPrice();

private:
    int ticketNumber;
    int flightNumber;
    int passportNumber;
    string seatNumber;
    double price;
    Ticket (int ticketNumber, int flightNumber, int passportNumber, string seatNumber, double price);
};

#endif // !TICKET_H
