class Flight {
public:
    Flight (time_t departureTime, time_t arrivalTime, string departureAirport, string arrivalAirport, int capacity) {
        this->flightNumber = chrono::system_clock::now().time_since_epoch().count();
        this->departureTime = departureTime;
        this->arrivalTime = arrivalTime;
        this->departureAirport = departureAirport;
        this->arrivalAirport = arrivalAirport;
        this->capacity = capacity;
    }

    // static vector<Flight> getAllFlights() {
    // TODO: implement getAllFlights
    // }

    // static Flight getFlightByFlightNumber(int flightNumber) {
    // TODO: implement getFlightByFlightNumber
    // }

    // static vector<Flight> getFlightByDepartureAirport(string departureAirport) {
    // TODO: implement getFlightByDepartureAirport
    // }

    // static vector<Flight> getFlightByArrivalAirport(string arrivalAirport) {
    // TODO: implement getFlightByArrivalAirport
    // }

    // static vector<Flight> getFlightByDepartureAirport(string departureAirport) {
    // TODO: implement getFlightByDepartureAirport
    // }

    // flightNumber
    int getFlightNumber() {
        return flightNumber;
    }

    // departureTime
    time_t getDepartureTime() {
        return departureTime;
    }
    void setDepartureTime(time_t departureTime) {
        this->departureTime = departureTime;
    }

    // arrivalTime
    time_t getArrivalTime() {
        return arrivalTime;
    }
    void setArrivalTime(time_t arrivalTime) {
        this->arrivalTime = arrivalTime;
    }

    // departureAirport
    string getDepartureAirport() {
        return departureAirport;
    }
    void setDepartureAirport(string departureAirport) {
        this->departureAirport = departureAirport;
    }

    // arrivalAirport
    string getArrivalAirport() {
        return arrivalAirport;
    }
    void setArrivalAirport(string arrivalAirport) {
        this->arrivalAirport = arrivalAirport;
    }

    // capacity
    int getCapacity() {
        return capacity;
    }

    // Save to database
    void save() {
        DataBaseConnection& db = DataBaseConnection::getInstance();
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db.getDB(), "WRITE THE UPDATE STATEMENT HERE", -1, &stmt, 0);

        if (rc != SQLITE_OK) {
            std::cerr << "Cannot prepare statement: " << sqlite3_errmsg(db.getDB()) << std::endl;
            return;
        }


        db.executeStatement(stmt);
    }

private:
    int flightNumber;
    time_t departureTime;
    time_t arrivalTime;
    string departureAirport;
    string arrivalAirport;
    int capacity;
};