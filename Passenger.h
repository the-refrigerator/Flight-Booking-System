class Passenger {
public:
    Passenger (string firstName, string lastName, string address, string phoneNumber, string email, int age) {
        this->passportNumber = chrono::system_clock::now().time_since_epoch().count();
        this->firstName = firstName;
        this->lastName = lastName;
        this->address = address;
        this->phoneNumber = phoneNumber;
        this->email = email;
        this->age = age;

        // TODO: add to database
    }
    // static vector<Passenger> getAllPassengers() {
    //     // TODO: implement getAllPassengers
    //     return vector<Passenger>();
    // }

    // static Passenger getPassengerByPassportNumber(int passportNumber) {
    //     // TODO: implement getPassengerByPassportNumber
    //     // return Passenger();
    // }

    // static vector<Passenger> getPassengerByFirstName(string Name) {
    //      // TODO: implement getPassengerByFirstName
    // }

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