#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <vector>
#include "model.h"

using namespace std;

class Passenger : public Model {
public:
    static Passenger* create(string firstName, string lastName, string address, string phoneNumber, string email, int age);
    static Passenger* AddPassenger();
    void AddTicket();
    void DeleteTicket();
    void FindFlight();
    static vector<Passenger> getAllPassengers();
    static void printAllPassengers();
    static Passenger* getPassengerByPassportNumber(int passportNumber);
    static Passenger* getPassengerByEmail(string email);
    static Passenger* getPassengerByPhoneNumber(string Number);
    void printPassenger();
    void remove();
    void save() override;
    int getPassportNumber();
    string getFirstName();
    void setFirstName(string firstName);
    string getLastName();
    void setLastName(string lastName);
    string getAddress();
    void setAddress(string address);
    string getPhoneNumber();
    void setPhoneNumber(string phoneNumber);
    string getEmail();
    void setEmail(string email);
    int getAge();
    void setAge(int age);

private:
    int passportNumber;
    string firstName;
    string lastName;
    string address;
    string phoneNumber;
    string email;
    int age;
    int flightNumber = 0;
    Passenger (int passportNumber, string firstName, string lastName, string address, string phoneNumber, string email, int age);
};

#endif // !PASSENGER_H
