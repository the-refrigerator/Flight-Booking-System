#include <iostream>
#include <sqlite3.h>
#include <string>

#include "Passenger.hpp"
#include "DataBaseConnection.hpp"



using namespace std;


int main() {
    Passenger* p = Passenger::create("John", "Doe", "123 Main St", "1234567890", "exmaple@email.com", 21);

    cout << p->getPassportNumber() << endl;
    cout << "Hello Worlde!!" << endl;
    return 0;
}

