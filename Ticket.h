class Ticket {
public:
    Ticket (int flightNumber, int passportNumber, string seatNumber, double price) {
        this->ticketNumber = chrono::system_clock::now().time_since_epoch().count();
        this->flightNumber = flightNumber;
        this->passportNumber = passportNumber;
        this->seatNumber = seatNumber;
        this->price = price;
    }

    // static vector<Ticket> getAllTicketsOfFlight(int flightNumber) {
    //  TODO: implement getAllTicketsOfFlight
    //}

    // static vector<Ticket> getAllTicketsOfPassenger(int passportNumber){
    // TODO: implement getAllTicketsOfPassenger
    // }

private:
    int ticketNumber;
    int flightNumber;
    int passportNumber;
    string seatNumber;
    double price;
};
