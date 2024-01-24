#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sqlite3.h>
#include <string>
#include <cctype>

#include "headers/Passenger.h"
#include "headers/Flight.h"
#include "headers/Director.h"
#include "headers/utils.h"

using namespace std;

const string password = "Airline167";

void clear_screen() {
    // http://stackoverflow.com/questions/17335816/ddg#32008479
    cout << "\033[2J\033[1;1H";
}

void PassengerAlgorithm() {
    char ans;
    int choice, number;
    string word;
    Passenger* p = nullptr;

    do {
        cout << "Have you been on our flights before?(y/n): ";
        cin >> ans;

        if(toupper(ans) == 'Y' || toupper(ans) == 'N')
            break;

        cin.clear();
        cin.ignore(10000, '\n');
    } while(true);

    clear_screen();

    if(ans == 'y' || ans == 'Y') {
        bool isWord = false;
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
            if(p->getFirstName() != "")
                p->printPassenger();

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

    while(true) {
        clear_screen();
        choice = -1;
        cout
                << "What would you like to do?" << endl
                << "\t1. Add ticket" << endl
                << "\t2. Delete Ticket" << endl
                << "\t3. Find a flight" << endl
                << "\t4. View your tickets" << endl
                << "\t0. Exit" << endl;
        cin >> choice;

        switch(choice) {
            case 1:
                p->AddTicket();
                break;

            case 2:
                p->DeleteTicket();
                break;

            case 3:
                p->Passenger::FindFlight();
                break;

            case 4:
                p->PrintTickets();
                break;

            case 0:
                cout << "Thank you for using our system!" << endl;
                exit(0);
                break;

            default:
                cout << "Invalid input!" << endl;
        }

        cout << "Press any key to continue..." << endl;
        cin.get();
        cin.ignore();
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

            if(toupper(status) == 'N' || toupper(status) == 'O')
                break;

            cin.clear();
            cin.ignore(10000, '\n');
        } while(true);


        if(toupper(status) == 'N')
            Director::AddDirector();


        while(true) {
            clear_screen();
            cout << endl
                 << "Would would you like to do?" << endl
                 << "\t1. Add a Flight" << endl
                 << "\t2. Remove a Flight" << endl
                 << "\t3. Add Passenger" << endl
                 << "\t4. Remove Passenger" << endl
                 << "\t5. View Flights" << endl
                 << "\t0. Exit" << endl;
            cin >> ans;

            switch(ans) {
                case 1:
                    Director::AddFlight();
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

                case 5:
                    Flight::PrintFlights();
                    break;

                case 0:
                    cout << "Thank you for using our system!" << endl;
                    exit(0);
                    break;

                default:
                    cout << "Invalid input!" << endl;
            }

            cout << "Press any key to continue..." << endl;
            cin.get();
            cin.ignore();
        }
    } else
        cout << "Wrong password!"<<endl;

}

int main() {
    char person = 'k';

    do {
        cout << "Are you a passenger or a director? (p/d) ";
        cin >> person;

        if (toupper(person) == 'P' || toupper(person) == 'D')
            break;

        cin.clear();
        cin.ignore(10000, '\n');
    } while(true);

    if (toupper(person) == 'P')
        PassengerAlgorithm();
    else if (toupper(person) == 'D')
        DirectorAlgorithm();

    return 0;
}

