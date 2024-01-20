#include <cstdlib>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <cctype>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cmath>

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
    } while(toupper(ans) != 'N' && toupper(ans) !='Y'); 

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

        if(p->getFirstName() != "")
            p->printPassanger();
        else
            cout << "User not found!" << endl;
    }
    else{
        cout<<"Adding a new user:"<<endl<<"Please enter the following:"<<endl;
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
            p->printPassanger();
    }

}



void DirectorAlgorithm() {
    string pass;
    char ans;
    cout << "Enter password to verify your identity: ";
    cin >> pass;

    if(pass == password) {
        char status;
        string Fname, Lname, Address, Number, Email;
        int Age;
        bool isEmail = false;

        do {
            cout << "Are you new or old? (n/o) ";
            cin >> status;
        } while(toupper(status) != 'N' && toupper(status) != 'O');


        if(toupper(status) == 'N') {
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
        }

        Flight::PrintFlights();

        do{
        cout<<endl<<"Would you like to add a flight? (y/n): ";
        cin>>ans;
        }while(toupper(ans)!='Y' && toupper(ans)!='N'); 

        if(toupper(ans)=='Y'){
            string arrA, deptA, arrT, depT;
            time_t t_arrT, t_depT, t_additional;
            tm timeInfo={};
            int cap;

            cout<<"Enter the following: "<<endl;

            cout<<"Departure Airport: ";
            cin>>deptA;

            cout<<"Arrival Airport: ";
            cin>>arrA;

            cout<<"Capacity: ";
            cin>>cap;

            cout<<"Departure Time: ";
            cin>>depT;
            istringstream s1(depT);
            s1>>get_time(&timeInfo, "%I:%M %p");
            t_depT=mktime(&timeInfo);
            cout<<"departure time: "<<t_depT<<endl;

            try{
                cin>>arrT;
            cout<<"Arrival Time: ";
            cin>>arrT;

            istringstream s2(arrT);
            s2>>get_time(&timeInfo, "%I:%M %p");
            t_arrT=mktime(&timeInfo);
            cout<<"arrival time: "<<t_arrT<<endl;
            

            istringstream s3("00:59");
            s3>>get_time(&timeInfo, "%I:%M");
            t_additional=mktime(&timeInfo);
            cout<<"additional: "<<t_additional<<endl;

            if(abs(t_arrT)< abs(t_depT)+ abs(t_additional))
                throw("impossible flight");
            }

            
            catch(const char* s){
                cout<<s<<endl;
            }
            Flight::create(t_depT, t_arrT, deptA, arrA, cap);

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
    cout << "Hello Worlde!!" << endl;
    return 0;
}

