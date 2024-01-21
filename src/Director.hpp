#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <sqlite3.h>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <iomanip>
#include "model.hpp"
#include "utils.hpp"
#include "Flight.hpp"
#include "Passenger.hpp"


using namespace std;

class Director : public Model {
public:
    static Director* create(string firstName, string lastName, string address, string phoneNumber, string email, int age) {
        // Execute an INSERT query to add a new Director to the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "INSERT INTO directors (idNumber, firstName, lastName, address, phoneNumber, email, age) VALUES (?, ?, ?, ?, ?, ?, ?)";
        long rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        long idNumber = abs(chrono::system_clock::now().time_since_epoch().count());

        sqlite3_bind_int(stmt, 1, idNumber);
        sqlite3_bind_text(stmt, 2, firstName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, lastName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, address.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, phoneNumber.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 7, age);

        db.executeStatement(stmt);

        return new Director(
                   idNumber,
                   firstName,
                   lastName,
                   address,
                   phoneNumber,
                   email,
                   age
               );
    }

    static Director* AddDirector() {
        string Fname, Lname, Address, Number, Email;
        int Age;
        bool isEmail = false;


        cout << "Please fill the following:" << endl;

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


        Director* d = Director::create(Fname, Lname, Address, Number, Email, Age);
        d->printDirector();
        return d;
    }

    static void AddFlight() {
        string arrA, deptA, arrT, depT;
        time_t t_arrT, t_depT, t_additional;
        tm timeInfo = {};
        int cap;

        cout << "Enter the following: " << endl;

        cout << "Departure Airport: ";
        cin >> deptA;

        cout << "Arrival Airport: ";
        cin >> arrA;

        cout << "Capacity: ";
        cin >> cap;

        cout << "Departure Time: ";
        cin >> depT;
        istringstream s1(depT);
        s1 >> get_time(&timeInfo, "%I:%M %p");
        t_depT = mktime(&timeInfo);
        cout << "departure time: " << t_depT << endl;

        try {
            cin >> arrT;
            cout << "Arrival Time: ";
            cin >> arrT;

            istringstream s2(arrT);
            s2 >> get_time(&timeInfo, "%I:%M %p");
            t_arrT = mktime(&timeInfo);
            cout << "arrival time: " << t_arrT << endl;


            istringstream s3("00:59");
            s3 >> get_time(&timeInfo, "%I:%M");
            t_additional = mktime(&timeInfo);
            cout << "additional: " << t_additional << endl;

            if(abs(t_arrT) < abs(t_depT) + abs(t_additional))
                throw("impossible flight");
        }


        catch(const char* s) {
            cout << s << endl;
            exit(0);
        }

        Flight::create(t_depT, t_arrT, deptA, arrA, cap);
    }

    static void RemoveFlight() {
        Flight::PrintFlights();
        vector<Flight> flights = Flight::getAllFlights();

        int choice;

        do{
            cout<<"Select a flight to remove: ";
            cin >> choice;
        } while(choice < 0 || choice > static_cast<int>flights.size());
        
        Flight* f=flights[choice-1];
        f->remove();
        delete() f;
        }

    static void RemovePassenger() {
        Passenger::printAllPassengers();

        vector<Passenger> passengers = Passenger::getAllPassengers();

        int choice;

        do{
            cout<<"Select a passenger to remove: ";
            cin >> choice;
        } while(choice < 0 || choice > static_cast<int>passengers.size());
        
        Passenger* p=passengers[choice-1];
        p->remove();
        delete() p;
    }

    static vector<Director> getAllDirectors() {
        vector<Director> Directors;

        // Execute a SELECT query to fetch all Directors from the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "SELECT * FROM directors";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int idNumber = sqlite3_column_int(stmt, 0);
            const char* firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const char* address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* phoneNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            int age = sqlite3_column_int(stmt, 6);

            Directors.push_back(
                Director(
                    idNumber,
                    firstName,
                    lastName,
                    address,
                    phoneNumber,
                    email,
                    age
                )
            );
        }

        sqlite3_finalize(stmt);

        return Directors;
    }

    static Director getDirectorByidNumebr(int idNumber) {

        // Execute a SELECT query to fetch a specific Director from the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "SELECT * FROM directors WHERE idNumber = ?";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_int(stmt, 1, idNumber);

        if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            const char* firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const char* address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            const char* phoneNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            int age = sqlite3_column_int(stmt, 6);

            Director director = Director(idNumber, firstName, lastName, address, phoneNumber, email, age);
            sqlite3_finalize(stmt);

            return director;
        } else
            return Director(0, "", "", "", "", "", 0);
    }

    void remove() {
        // Execute a DELETE query to remove the Director from the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "DELETE FROM directors WHERE idNumber = ?";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_int(stmt, 1, idNumber);

        db.executeStatement(stmt);
    }

    void save() override {
        // Execute an UPDATE query to modify the Director information in the database
        // Use the database connection from DataBaseConnection singleton
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;

        const char* query = "UPDATE directors SET firstName = ?, lastName = ?, address = ?, phoneNumber = ?, email = ?, age = ? WHERE idNumber = ?";
        int rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
            throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

        sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, address.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, phoneNumber.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, age);
        sqlite3_bind_int(stmt, 7, idNumber);

        db.executeStatement(stmt);
    }

    void printDirector() {
        cout << endl << endl << "Hello Director " << this->firstName << " " << this->lastName << endl << "ID Number: " << this->idNumber << endl << "Age: " << this->age << endl << "Address: " << address << endl << "PhoneNumber: " << this->phoneNumber << endl << "email: " << this->email << endl << endl;
    }

    // idNumber
    int getIdNumber() {
        return idNumber;
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
    int idNumber;
    string firstName;
    string lastName;
    string address;
    string phoneNumber;
    string email;
    int age;

    Director (int idNumber, string firstName, string lastName, string address, string phoneNumber, string email, int age) {
        this->idNumber = idNumber;
        this->firstName = firstName;
        this->lastName = lastName;
        this->address = address;
        this->phoneNumber = phoneNumber;
        this->email = email;
        this->age = age;
    }

};

#endif // !DIRECTOR_H
