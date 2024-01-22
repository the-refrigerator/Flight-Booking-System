#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <string>
#include <vector>
#include "DataBaseConnection.h"
#include "model.h"

using namespace std;

class Director : public Model {
public:
    static Director* create(string firstName, string lastName, string address, string phoneNumber, string email, int age);
    static Director* AddDirector();
    static void AddFlight();
    static void RemoveFlight();
    static void RemovePassenger();
    static vector<Director> getAllDirectors();
    static Director getDirectorByidNumebr(int idNumber);
    void remove();
    void save() override;
    void printDirector();
    int getIdNumber();
    string getFirstName();
    void setFirstName(string firstName);
    string getLastName();
    void setLastName(string lastName);
    string getAddress();
    void setAddress(string address);
    string getPhoneNumber();
    void setPhoneNumber(string phoneNumber);
    string getEmail();
    void setEmail(string email);
    int getAge();
    void setAge(int age);

private:
    int idNumber;
    string firstName;
    string lastName;
    string address;
    string phoneNumber;
    string email;
    int age;
    Director (int idNumber, string firstName, string lastName, string address, string phoneNumber, string email, int age);
};

#endif // !DIRECTOR_H
