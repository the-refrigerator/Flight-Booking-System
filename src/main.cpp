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
using namespace chrono;

const string password = "Airline167";

void PassengerAlgorithm() {
    char ans;
    int choice, number;
    string word;
    Passenger* p = nullptr;

    do {
        cout << "Have you been on our flights before?(y/n): ";
        cin >> ans;
    } while(toupper(ans) != 'N' && toupper(ans) != 'Y');

    if(ans == 'y' || ans == 'Y') {
        bool  isWord = false;
        cout << "Choose method of search:\n1. Passport Number\n2. email\n3. Phone Number" << endl;
        cin >> choice;

        if(choice == 1) {
            cout << "Please enter your passport number: " << endl;
            cin >> number;
            p = Passenger::getPassengerByPassportNumber(number);
        } else if(choice == 2) {
            do {
                getline(cin, word);
                cout << "Please enter your email: " << endl;
                getline(cin, word);
                isWord = Utils::verify_email(word);
            } while(isWord == false);

            p = Passenger::getPassengerByEmail(word);

        } else if(choice == 3) {
            getline(cin, word);
            cout << "Please enter your phone number(+### ###...): " << endl;
            getline(cin, word);
            p = Passenger::getPassengerByPhoneNumber(word);
            cout << p->getFirstName() << endl;
        }

        try {
            if(p->getFirstName() != ""){
                p->printPassenger();
                cout<<"You are booked to: "<<endl;}
            else
                throw "User not found!";
        }

        catch(const char* s) {
            cerr << s << endl;
            exit(0);

        }

    } else {
        cout << "Adding a new user:" << endl;
        p = Passenger::AddPassenger();
    }

    do {
        cout << "What would you like to do?" << endl << "1. Add ticket" << endl << "2. Delete Ticket" << endl << "3. Find a flight" << endl;
        cin >> choice;
    } while(choice != 1 && choice != 2 && choice != 3);


    switch(choice) {
        case 1:
            p->AddTicket();
            break;

        case 2:
            p->DeleteTicket();
            break;

        default:
            p->Passenger::FindFlight();
    }

}



void DirectorAlgorithm() {
    string pass;
    int ans;
    cout << "Enter password to verify your identity: ";
    cin >> pass;

    if(pass == password) {
        char status;

        do {
            cout << "Are you new or old? (n/o) ";
            cin >> status;
        } while(toupper(status) != 'N' && toupper(status) != 'O');


        if(toupper(status) == 'N')
            Director* d = Director::AddDirector();


        do {
            cout << endl << "Would would you like to do?" << endl << "1. Add a Flight" << endl << "2.Remove a Flight" << endl << "3.Add Passenger" << endl << "4.Remove Passenger" << endl << "5. View Flights" << endl;
            cin >> ans;
        } while(ans != 1 && ans != 2 && ans != 3 && ans != 4 && ans != 5);


        switch(ans) {
            case 1:
                // Director::AddFlight();
                break;

            case 2:
                Director::RemoveFlight();
                break;

            case 3:
                Passenger::AddPassenger();
                break;

            case 4:
                Director::RemovePassenger();
                break;

            default:
                Flight::PrintFlights();
        }

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
    } while(toupper(person) != 'P' && toupper(person) != 'D');

    if (toupper(person) == 'P')
        PassengerAlgorithm();
    else if (toupper(person) == 'D')
        DirectorAlgorithm();

    // Passenger* p = Passenger::create("John", "Doe", "123 Main St", "1234567890", "exmaple@email.com", 21);

    // cout << p->getPassportNumber() << endl;
    cout << endl << "Hello Worlde!!" << endl;
    return 0;
}

