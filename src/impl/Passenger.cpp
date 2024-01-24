#include <sqlite3.h>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../headers/Passenger.h"
#include "../headers/utils.h"
#include "../headers/Flight.h"
#include "../headers/Ticket.h"

using namespace std;
Passenger* Passenger::create(string firstName, string lastName, string address, string phoneNumber, string email, int age) {
    // Execute an INSERT query to add a new passenger to the database
    // Use the database connection from DataBaseConnection singleton
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "INSERT INTO passengers (passportNumber, firstName, lastName, address, phoneNumber, email, age) VALUES (?, ?, ?, ?, ?, ?, ?)";
    long rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    int passportNumber = abs(static_cast<int>(abs(chrono::system_clock::now().time_since_epoch().count())));

    sqlite3_bind_int(stmt, 1, passportNumber);
    sqlite3_bind_text(stmt, 2, firstName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, lastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, phoneNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, age);

    db.executeStatement(stmt);

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

Passenger* Passenger::AddPassenger() {
    cout << "Please enter the following:" << endl;
    string Fname, Lname, Address, Number, Email, strAge;
    int Age, number;
    bool isEmail = false, validinput = false;

    getline(cin, Fname);
    cout << "Name: ";
    getline(cin, Fname);

    cout << "Last name: ";
    getline(cin, Lname);

    cout << "Address: ";
    getline(cin, Address);


    do{
        cout << "Age: ";
        getline(cin, strAge);
        istringstream iss(strAge);
        if(iss >> number){
            validinput = true;
            Age=stoi(strAge);            

        }
    }while (validinput == false);
    

    getline(cin, Number);
    cout << "Phone Number: ";
    getline(cin, Number);

    do {
        cout << "Email: ";
        getline(cin, Email);
        isEmail = Utils::verify_email(Email);
    } while(isEmail == false);

    Passenger* p = Passenger::create(Fname, Lname, Address, Number, Email, Age);
    p->printPassenger();
    return p;
}

void Passenger::AddTicket() {
    vector<Flight> availableFlights = Flight::getAllFlights();

    if(availableFlights.size() == 0) {
        cout << "No flights available!" << endl;
        return;
    }

    Flight::PrintFlights();
    Flight* f = nullptr;

    int choice;
    string seat;

    while(true) {
        do {
            cout << "Choose a Flight to book: ";
            cin >> choice;
        } while(choice > static_cast<int>(availableFlights.size()));

        f = &availableFlights[choice - 1];

        if(f->numberOfTickets() < f->getCapacity())
            break;
        else
            cout << "Flight full! Please choose another flight." << endl;
    }

    cout << "Please choose a seat: ";
    cin >> seat;

    try {
        Ticket::create(flightNumber, passportNumber, seat, 0.0);
    } catch(const char* s) {
        cout << s;
        exit(0);
    }

}

void Passenger::DeleteTicket() {
    vector<Ticket> tickets = Ticket::getAllTicketsOfPassenger(passportNumber);
    int ticketNumber;

    if(tickets.size() == 0) {
        cout << "No tickets found!" << endl;
        return;
    }

    for(int i = 0; i < static_cast<int>(tickets.size()); i++)
        cout
                << i + 1 << ". "
                << tickets[i].getTicketNumber() << "\t"
                << tickets[i].getFlightNumber() << "\t"
                << tickets[i].getSeatNumber() << endl;

    while(true) {
        cout << "Please choose a ticket to delete: ";
        cin >> ticketNumber;

        if(ticketNumber <= 0)
            return;

        for(int i = 0; i < static_cast<int>(tickets.size()); i++)
            if(tickets[i].getTicketNumber() == ticketNumber)
                break;

        cout << "Invalid ticket number!" << endl;
        return;
    }

    Ticket* t = Ticket::getTicketByTicketNumber(ticketNumber);

    if(t->getTicketNumber() == 0) {
        cout << "Ticket not found!" << endl;
        return;
    }

    t->remove();
}

void Passenger::FindFlight() {
    string input;
    char choice;
    vector<Flight> availableFlights;
    Flight* f = nullptr;

    cout << "Would you like to search by destination or departure? ";
    cin >> input;

    while(true) {
        if(input == "destination") {
            cout << "Please enter the destination: ";
            cin >> input;
            availableFlights = Flight::getFlightByArrivalAirport(input);
            break;
        } else if(input == "departure") {
            cout << "Please enter the departure location: ";
            cin >> input;
            availableFlights = Flight::getFlightByDepartureAirport(input);
            break;
        } else
            cout << "Invalid input" << endl;
            
    }

    if(availableFlights.size() > 0) {
        Flight::PrintFlightsVector(availableFlights);

        do {
            cout << "Would you like to book a flight(y/n)? ";
            cin >> choice;
        } while(toupper(choice) != 'Y' && toupper(choice) != 'N');

        if(toupper(choice) == 'Y') {
            int flightChoice;

            cout << "Please choose a flight: ";
            cin >> flightChoice;

            if(flightChoice > 0 && flightChoice <= static_cast<int>(availableFlights.size())) {

                f = &availableFlights[flightChoice - 1];

                string seat;

                cout << "Please choose a seat: ";
                cin >> seat;

                if(f->numberOfTickets() < f->getCapacity())
                    Ticket::create(f->getFlightNumber(), passportNumber, seat, 0.0);

                else {
                    cout << "Flight full!";
                    exit(0);
                }
            } else {
                cout << "Invalid choice" << endl;
                exit(0);
            }

        } else
            exit(0);
    } else {
        cout << "No flights found" << endl;
        exit(0);
    }
}

void Passenger::PrintTickets() {
    vector<Ticket> tickets = Ticket::getAllTicketsOfPassenger(passportNumber);

    if(tickets.size() == 0) {
        cout << "No tickets found!" << endl;
        return;
    }

    for(int i = 0; i < static_cast<int>(tickets.size()); i++)
        cout
                << i + 1 << ". "
                << tickets[i].getTicketNumber() << "\t"
                << tickets[i].getFlightNumber() << "\t"
                << tickets[i].getSeatNumber() << endl;
}

vector<Passenger> Passenger::getAllPassengers() {
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

    return passengers;
}

void Passenger::printAllPassengers() {
    vector<Passenger> passengers = getAllPassengers();

    cout << endl << "List of Passengers in the Airline: " << endl << "Passenger\tPassport Number\t\tName\t\tLast Name\t\tAddress\t\t\tEmail\t\t\t\tPhone Number\t\tAge" << endl;

    for(int i = 0; i < static_cast<int>(passengers.size()); i++)
        cout << i + 1 << "\t\t" << passengers[i].passportNumber << "\t\t" << passengers[i].firstName << "\t\t" << passengers[i].lastName << "\t\t\t" << passengers[i].address << "\t\t\t" << passengers[i].email << "\t\t" << passengers[i].phoneNumber << "\t\t" << passengers[i].age << endl;
}

Passenger* Passenger::getPassengerByPassportNumber(int passportNumber) {
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "SELECT * FROM passengers WHERE passportNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    sqlite3_bind_int(stmt, 1, passportNumber);

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int passportNumber = sqlite3_column_int(stmt, 0);
        const char* firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* phoneNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        int age = sqlite3_column_int(stmt, 6);


        return new Passenger(
                   passportNumber,
                   firstName,
                   lastName,
                   address,
                   phoneNumber,
                   email,
                   age
               );
    } else
        return new Passenger(0, "", "", "", "", "", 0);
}

Passenger* Passenger::getPassengerByEmail(string email) {
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "SELECT * FROM passengers WHERE email = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int passportNumber = sqlite3_column_int(stmt, 0);
        const char* firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* phoneNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        int age = sqlite3_column_int(stmt, 6);

        return new Passenger(
                   passportNumber,
                   firstName,
                   lastName,
                   address,
                   phoneNumber,
                   email,
                   age
               );
    } else
        return new Passenger(0, "", "", "", "", "", 0);
}

Passenger* Passenger::getPassengerByPhoneNumber(string Number) {
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "SELECT * FROM passengers WHERE phoneNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    sqlite3_bind_text(stmt, 1, Number.c_str(), -1, SQLITE_STATIC);

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int passportNumber = sqlite3_column_int(stmt, 0);
        const char* firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* phoneNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        int age = sqlite3_column_int(stmt, 6);

        return new Passenger(
                   passportNumber,
                   firstName,
                   lastName,
                   address,
                   phoneNumber,
                   email,
                   age
               );
    } else
        return new Passenger(0, "", "", "", "", "", 0);
}

void Passenger::printPassenger() {
    cout << endl << endl << "Hello Passenger " << this->firstName << " " << this->lastName << endl << "Passport Number: " << this->passportNumber << endl << "Age: " << this->age << endl << "Address: " << address << endl << "PhoneNumber: " << this->phoneNumber << endl << "email: " << this->email << endl << endl;
}

void Passenger::remove() {
    // Execute a DELETE query to remove the passenger from the database
    // Use the database connection from DataBaseConnection singleton
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    for(Ticket t : Ticket::getAllTicketsOfPassenger(this->passportNumber))
        t.remove();

    const char* query = "DELETE FROM passengers WHERE passportNumber = ?";
    int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    sqlite3_bind_int(stmt, 1, passportNumber);

    db.executeStatement(stmt);
}

void Passenger::save() {
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
}

// passportNumber
int Passenger::getPassportNumber() {
    return passportNumber;
}

// firstName
string Passenger::getFirstName() {
    return firstName;
}
void Passenger::setFirstName(string firstName) {
    this->firstName = firstName;
}

// lastName
string Passenger::getLastName() {
    return lastName;
}
void Passenger::setLastName(string lastName) {
    this->lastName = lastName;
}

// address
string Passenger::getAddress() {
    return address;
}
void Passenger::setAddress(string address) {
    this->address = address;
}

// phoneNumber
string Passenger::getPhoneNumber() {
    return phoneNumber;
}
void Passenger::setPhoneNumber(string phoneNumber) {
    this->phoneNumber = phoneNumber;
}

// email
string Passenger::getEmail() {
    return email;
}
void Passenger::setEmail(string email) {
    this->email = email;
}

// age
int Passenger::getAge() {
    return age;
}
void Passenger::setAge(int age) {
    this->age = age;
}

Passenger::Passenger (int passportNumber, string firstName, string lastName, string address, string phoneNumber, string email, int age) {
    this->passportNumber = passportNumber;
    this->firstName = firstName;
    this->lastName = lastName;
    this->address = address;
    this->phoneNumber = phoneNumber;
    this->email = email;
    this->age = age;
}
