#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <cctype>

#include "Passenger.hpp"
#include "DataBaseConnection.hpp"
#include "model.hpp"
#include "Flight.hpp"
#include "Ticket.hpp"
#include "Director.hpp"
#include "utils.hpp"

using namespace std;

const string password = "Airline167";

void PassengerAlgorithm() {
    char ans;
    int choice, number;
    string word;
    // Passenger* p=nullptr;
    Passenger* p = Passenger::constr();
    // Passenger* p2 = Passenger::create("Jon", "Snow", "The north", "945", "iknownth@jonsnow.dragons", 25);

    do {
        cout << "Have you been on our flights before?(y/n): ";
        cin >> ans;
    } while(ans != 'n' && ans != 'N' && ans != 'y' && ans != 'Y');

    if(ans == 'y' || ans == 'Y') {
        bool  isWord=false;
        cout << "Choose method of search:\n1. Passport Number\n2. email\n3. Phone Number" << endl;
        cin >> choice;

        if(choice == 1) {
            cout << "Please enter your passport number: " << endl;
            cin >> number;
            p->getPassengerByPassportNumber(number);
        } else if(choice == 2) {

            do{
                            getline(cin, word);

            cout << "Please enter your email: " << endl;
            getline(cin, word);
            isWord=Utils::verify_email(word);
            }while(isWord==false);

            p->getPassengerByEmail(word);

        } else if(choice == 3) {
                        getline(cin, word);
            cout << "Please enter your phone number(+### ###...): " << endl;
            getline(cin, word);
            p->getPassengerByPhoneNumber(word);
        }

        if(p->getFirstName() != "")
            p->printPassanger();
        else
            cout<<"User not found!"<<endl;
    }


}

void DirectorAlgorithm() {
    string pass;

    cout << "Enter password to verify your identity: ";
    cin >> pass;

    if(pass == password) {
        char status;
        string Fname, Lname, Address, Number, Email;
        int Age;
        bool isEmail=false;

        do {
            cout << "Are you new or old? (n/o) ";
            cin >> status;
        } while(toupper(status) != 'N' && toupper(status) != 'O');


        if(status == 'N' || status == 'n') {
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

            do{
            cout << "Email: ";
            getline(cin, Email);
            isEmail=Utils::verify_email(Email);
            }while(isEmail==false);

            Director* d = Director::create(Fname, Lname, Address, Number, Email, Age);
            d->printDirector();
        }

        cout<<"List of flights: "<<endl<<endl;
        cout<<"Flight Number\tDeparture Airport\tDeparture Time\tArrival Airport\tArrival Time\tCapacity"<<endl;
    } else
        cout << "Wrong password!";

}

int main() {
    string input;
    char person = 'k';

    do {
        cin.clear();
        cout << "enter 0000 to exit: ";
        getline(cin, input);
    } while (!cin);

    if (input == "0000")
        exit(0);

    do {
        cout << "Are you a passenger or a director? (p/d) ";
        cin >> person;
    } while(person != 'p' && person != 'P' && person != 'd' && person != 'D');

    if (person == 'p' || person == 'P')
        PassengerAlgorithm();
    else if (person == 'd' || person == 'D')
        DirectorAlgorithm();

    // Passenger* p = Passenger::create("John", "Doe", "123 Main St", "1234567890", "exmaple@email.com", 21);

    // cout << p->getPassportNumber() << endl;
    cout << "Hello Worlde!!" << endl;
    return 0;
}

