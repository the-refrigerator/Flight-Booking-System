#ifndef PASSENGER_H
#define PASSENGER_H

#include <sqlite3.h>
#include <chrono>
#include <string>
#include <vector>
#include "model.hpp"
#include "utils.hpp"
#include "Flight.hpp"

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

    static Passenger* AddPassenger() {
        cout << "Please enter the following:" << endl;
        string Fname, Lname, Address, Number, Email;
        int Age;
        bool isEmail = false;

        getline(cin, Fname);
        cout << "Name: ";
        getline(cin, Fname);

        cout << "Last name: ";
        getline(cin, Lname);

        cout << "Address: ";
        getline(cin, Address);

        cout << "Age: ";
        cin >> Age;

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


    void AddTicket() {

        if(flightNumber == 0) {
            vector<Flight> availableFlights = Flight::getAllFlights();
            Flight::PrintFlights();
            Flight* f = nullptr;

            int choice;

            do {
                cout << "Choose a Flight to book: ";
                cin >> choice;
            } while(choice > static_cast<int>(availableFlights.size()));

            f = &availableFlights[choice - 1];

            try {
                if(f->getNumberOfTickets() < f->getCapacity()) {
                    this->flightNumber = f->getFlightNumber();
                    f->incrementTickets();
                } else
                    throw "Flight full!";
            }

            catch(const char* s) {
                cout << s;
                exit(0);
            }


        } else {
            cout << "You're already booked to flight " << flightNumber << endl;
            exit(0);
        }
    }

    void DeleteTicket() {
        if(flightNumber != 0) {
            Flight f = Flight::getFlightByFlightNumber(flightNumber);
            f.decrementTickets();
            flightNumber = 0;
        } else {
            cout << "You're not booked to any flight!" << endl;
            exit(0);
        }
    }

    void FindFlight(){
        string input;
        char choice;
        vector<Flight> availableFlights;
        Flight* f=nullptr;

        cout<<"Would you like to search by destination or departure? ";
        cin>>input;

        if(input=="destination"){
                cout<<"Please enter the destination: ";
                cin>>input;
                availableFlights=Flight::getFlightByArrivalAirport(input);
        }    
        else if(input== "departure"){
                cout<<"Please enter the departure location: ";
                cin>>input;
                availableFlights=Flight::getFlightByDepartureAirport(input);

            }
            else{
                cout<<"Invalid input"<<endl;
                exit(0);
            }
        
        
        if(availableFlights.size()>0){
            Flight::PrintFlightsVector(availableFlights);
        do{
            cout<<"Would you like to book a flight(y/n)? ";
            cin>>choice;
        }while(toupper(choice)!='Y' && toupper(choice)!='N');

        if(toupper(choice)=='Y'){
            int flightChoice;

            cout<<"Please choose a flight: ";
            cin>>flightChoice;

            if(flightChoice>0 && flightChoice<=static_cast<int>(availableFlights.size())){

                f=&availableFlights[flightChoice-1];
                if(f->getNumberOfTickets() < f->getCapacity()) {
                        this->flightNumber = f->getFlightNumber();
                        f->incrementTickets();
                    } else{
                        cout<< "Flight full!";
                        exit(0);
                    }
            }
            else{
                cout<<"Invalid choice"<<endl;
                exit(0);
            }
            
        }
        else
            exit(0);
        }
        else{
            cout<<"No flights found"<<endl;
            exit(0);
        }
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

        return passengers;
    }

    static void printAllPassengers() {
        vector<Passenger> passengers = getAllPassengers();

        cout << endl << "List of Passengers in the Airline: " << endl << "Passenger\tPassport Number\t\tName\t\tLast Name\t\tAddress\t\t\tEmail\t\t\t\tPhone Number\t\tAge" << endl;

        for(int i = 0; i < static_cast<int>(passengers.size()); i++)
            cout << i + 1 << "\t\t" << passengers[i].passportNumber << "\t\t" << passengers[i].firstName << "\t\t" << passengers[i].lastName << "\t\t\t" << passengers[i].address << "\t\t\t" << passengers[i].email << "\t\t" << passengers[i].phoneNumber << "\t\t" << passengers[i].age << endl;
    }

    static Passenger* getPassengerByPassportNumber(int passportNumber) {
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

    static Passenger* getPassengerByEmail(string email) {
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

    static Passenger* getPassengerByPhoneNumber(string Number) {
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
    void printPassenger() {
        cout << endl << endl << "Hello Passenger " << this->firstName << " " << this->lastName << endl << "Passport Number: " << this->passportNumber << endl << "Age: " << this->age << endl << "Address: " << address << endl << "PhoneNumber: " << this->phoneNumber << endl << "email: " << this->email << endl << endl;
    }


    void remove() {
        // Execute a DELETE query to remove the passenger from the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "DELETE FROM passengers WHERE passportNumber = ?";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_int(stmt, 1, passportNumber);

        db.executeStatement(stmt);
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
    int flightNumber = 0;

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
