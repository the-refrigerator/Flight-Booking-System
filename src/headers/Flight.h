#ifndef FLIGHT_H
#define FLIGHT_H

#include <vector>
#include <string>
#include "model.h"
#include "DataBaseConnection.h"

using namespace std;

class Flight : public Model {
public:
    static Flight* create(string departureTime, string arrivalTime, string departureAirport, string arrivalAirport, int capacity);
    static vector<Flight> getAllFlights();
    static Flight getFlightByFlightNumber(int flightNumber);
    static vector<Flight> getFlightByDepartureAirport(string departureAirport);
    static vector<Flight> getFlightByArrivalAirport(string arrivalAirport);
    static void PrintFlights();
    static void PrintFlightsVector(vector<Flight> flights);
    void PrintFlight();
    int numberOfTickets();
    int getFlightNumber();
    string getDepartureTime();
    void setDepartureTime(string departureTime);
    string getArrivalTime();
    void setArrivalTime(string arrivalTime);
    string getDepartureAirport();
    void setDepartureAirport(string departureAirport);
    string getArrivalAirport();
    void setArrivalAirport(string arrivalAirport);
    int getCapacity();
    int getNumberOfTickets();
    void incrementTickets();
    void decrementTickets();
    void remove();
    void save() override;

private:
    int flightNumber;
    string departureTime;
    string arrivalTime;
    string departureAirport;
    string arrivalAirport;
    int capacity;
    Flight(int flightNumber, string departureTime, string arrivalTime, string departureAirport, string arrivalAirport, int capacity);
};

#endif // !FLIGHT_H
