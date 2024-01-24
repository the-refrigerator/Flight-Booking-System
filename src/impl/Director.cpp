#include <ostream>
#include <sqlite3.h>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../headers/Director.h"
#include "../headers/utils.h"
#include "../headers/Flight.h"
#include "../headers/Passenger.h"
#include "../headers/Ticket.h"

using namespace std;

Director* Director::create(string firstName, string lastName, string address, string phoneNumber, string email, int age) {
    // Execute an INSERT query to add a new Director to the database
    // Use the database connection from DataBaseConnection singleton
    DataBaseConnection& db = DataBaseConnection::getInstance();
    sqlite3_stmt* stmt;

    const char* query = "INSERT INTO directors (idNumber, firstName, lastName, address, phoneNumber, email, age) VALUES (?, ?, ?, ?, ?, ?, ?)";
    long rc = sqlite3_prepare_v2(db.getDB(), query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        throw runtime_error("Cannot prepare statement: " + string(sqlite3_errmsg(db.getDB())));

    //cast to long to avoid negative numbers
    int idNumber = abs(static_cast<int>(abs(chrono::system_clock::now().time_since_epoch().count())));

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

Director* Director::AddDirector() {
    string Fname, Lname, Address, Number, Email, strAge;
    int Age, number;
    bool isEmail = false, validinput = false;


    cout << "Please fill the following:" << endl;

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


    Director* d = Director::create(Fname, Lname, Address, Number, Email, Age);
    d->printDirector();
    return d;
}

void Director::AddFlight() {
    string arrA, deptA, arrT, depT, strCap;
    int cap, number;
    bool validinput=false;

    cout << "Enter the following: " << endl;

    getline(cin, deptA);
    cout << "Departure Airport: ";
    getline(cin, deptA);

    cout << "Arrival Airport: ";
    getline(cin, arrA);

    do{
        cout << "Capacity: ";
        getline(cin, strCap);
        istringstream iss(strCap);
        if(iss >> number){
            validinput = true;
            cap=stoi(strCap);            

        }
    }while (validinput == false);

    while(!cin || cap <= 0) {
        cout << "Invalid input!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cin >> cap;
    }

    getline(cin, depT);
    cout << "Departure Time: ";
    getline(cin, depT);

    try {
        cout << "Arrival Time: ";
        getline(cin, arrT);
    } catch(const char* s) {
        cout << s << endl;
        exit(1);
    }

    Flight::create(depT, arrT, deptA, arrA, cap);
}

void Director::RemoveFlight() {
    Flight::PrintFlights();
    vector<Flight> flights = Flight::getAllFlights();

    int choice;

    do {
        cout << "Select a flight to remove: ";
        cin >> choice;
    } while(choice < 0 || choice > static_cast<int>(flights.size()));

    Flight* f = &flights[choice - 1];
    f->remove();
}

void Director::RemovePassenger() {
    Passenger::printAllPassengers();

    vector<Passenger> passengers = Passenger::getAllPassengers();

    int choice;

    do {
        cout << "Select a passenger to remove: ";
        cin >> choice;
    } while(choice < 0 || choice > static_cast<int>(passengers.size()));

    Passenger* p = &passengers[choice - 1];
    p->Passenger::remove();
}

vector<Director> Director::getAllDirectors() {
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

Director Director::getDirectorByidNumebr(int idNumber) {

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

void Director::remove() {
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

void Director::save() {
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

void Director::printDirector() {
    cout << endl << endl << "Hello Director " << this->firstName << " " << this->lastName << endl << "ID Number: " << this->idNumber << endl << "Age: " << this->age << endl << "Address: " << address << endl << "PhoneNumber: " << this->phoneNumber << endl << "email: " << this->email << endl << endl;
}

// idNumber
int Director::getIdNumber() {
    return idNumber;
}

// firstName
string Director::getFirstName() {
    return firstName;
}
void Director::setFirstName(string firstName) {
    this->firstName = firstName;
}

// lastName
string Director::getLastName() {
    return lastName;
}
void Director::setLastName(string lastName) {
    this->lastName = lastName;
}

// address
string Director::getAddress() {
    return address;
}
void Director::setAddress(string address) {
    this->address = address;
}

// phoneNumber
string Director::getPhoneNumber() {
    return phoneNumber;
}
void Director::setPhoneNumber(string phoneNumber) {
    this->phoneNumber = phoneNumber;
}

// email
string Director::getEmail() {
    return email;
}
void Director::setEmail(string email) {
    this->email = email;
}

// age
int Director::getAge() {
    return age;
}
void Director::setAge(int age) {
    this->age = age;
}

Director::Director (int idNumber, string firstName, string lastName, string address, string phoneNumber, string email, int age) {
    this->idNumber = idNumber;
    this->firstName = firstName;
    this->lastName = lastName;
    this->address = address;
    this->phoneNumber = phoneNumber;
    this->email = email;
    this->age = age;
}

