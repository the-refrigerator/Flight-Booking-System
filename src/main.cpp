#include <iostream>
#include <sqlite3.h>
#include <string>

#include "Passenger.hpp"
#include "DataBaseConnection.hpp"
#include<cctype>
// #include "Flight.hpp"
//this file has errors

// #include "Ticket.hpp"
#include "Director.hpp"
#include<algorithm>

using namespace std;


void PassengerAlgorithm(){
    char ans;
    int choice, number;
    string word;
    // Passenger* p=nullptr;
    Passenger* p = Passenger::create("John", "Doe", "123 Main St", "1234567890", "exmaple@email.com", 21);
    cout<<"passport number: "<<p->getPassportNumber()<<endl;

    do{
    cout<<"Have you been on our flights before?(y/m): ";
    cin>>ans;
    }while(ans!='n' && ans!='N' && ans!='y' && ans!='Y');

    if(ans=='y' || ans=='Y'){
        cout<<"Choose method of search:\n1. Passport Number\n2. email\n3. Phone Number"<<endl;
        cin>>choice;

        if(choice==1){
            cout<<"Please enter your passport number: "<<endl;
            cin>>number;
            p->getPassengerByPassportNumber(number);
        }
        else if(choice==2){
            cout<<"Please enter your email: "<<endl;
            cin>>word;
            // p=getPassengerByEmail(word);
        }
        else if(choice==3){
             cout<<"Please enter your phone number(+### ###...): "<<endl;
            cin>>word;
            // p=getPassengerByPhoneNumber(word);
        }

        if(p->getFirstName()!="")
            p->printPassanger();

    }


}


int main() {

    int input;
    char person='k';
    const string password="Airline167";


    cout<<"enter 0000 to exit: "; /*idk how to terminate the program upon 0000*/
    cin>>input;

    // while(input!=0000){

        do{
        cout<<"Are you a passenger or a director? (p/d) ";
        cin>>person;
        }while(person!='p' && person!= 'P' && person!='d' && person!='D');

    if (person=='p' || person=='P')
        PassengerAlgorithm();
    
    
    // Passenger* p = Passenger::create("John", "Doe", "123 Main St", "1234567890", "exmaple@email.com", 21);

    // cout << p->getPassportNumber() << endl;
    cout << "Hello Worlde!!" << endl;
    return 0;
}

