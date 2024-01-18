--Create Tables

CREATE TABLE flights (
    flightNumber INTEGER PRIMARY KEY,
    departureTime INTEGER,
    arrivalTime INTEGER,
    departureAirport TEXT,
    arrivalAirport TEXT,
    capacity INTEGER
);

CREATE TABLE passengers (
    passportNumber INTEGER PRIMARY KEY,
    firstName TEXT,
    lastName TEXT,
    address TEXT,
    phoneNumber TEXT,
    email TEXT UNIQUE,
    age INTEGER
);

CREATE TABLE tickets (
    ticketNumber INTEGER PRIMARY KEY,
    flightNumber INTEGER,
    passportNumber INTEGER,
    seatNumber TEXT,
    price REAL,
    FOREIGN KEY (flightNumber) REFERENCES flights(flightNumber),
    FOREIGN KEY (passportNumber) REFERENCES passengers(passportNumber)
);

CREATE TABLE directors (
    idNumber INTEGER PRIMARY KEY,
    firstName TEXT,
    lastName TEXT,
    address TEXT,
    phoneNumber TEXT,
    email TEXT,
    age INTEGER
);

--Create Data
INSERT INTO flights (flightNumber, departureTime, arrivalTime, departureAirport, arrivalAirport, capacity)
VALUES (
    1,
    1000,
    1200,
    'LAX',
    'SFO',
    100
);

INSERT INTO passengers (passportNumber, firstName, lastName, address, phoneNumber, email, age)
VALUES (
    123456789,
    'John',
    'Doe',
    '123 Main St',
    '123-456-7890',
    'example@email.com',
    21
);

INSERT INTO tickets (ticketNumber, flightNumber, passportNumber, seatNumber, price)
VALUES (
    1,
    1,
    123456789,
    'A1',
    100.00
);

INSERT INTO directors (idNumber, firstName, lastName, address, phoneNumber, email, age)
VALUES (
    123456789,
    'admin',
    'admin',
    '123 Main St',
    '123-456-7890',
    'example@email.com',
    30
);
--Read Data
SELECT * FROM flights;
SELECT * FROM flights WHERE flightNumber = 1;

SELECT * FROM passengers;
SELECT * FROM passengers WHERE passportNumber = 123456789;
SELECT * FROM passengers WHERE firstName = 'John';

SELECT * FROM tickets;
SELECT * FROM tickets WHERE ticketNumber = 1;
SELECT * FROM tickets WHERE flightNumber = 1;
SELECT * FROM tickets WHERE passportNumber = 123456789;

SELECT * FROM directors;
SELECT * FROM directors WHERE idNumber = 123456789;
SELECT * FROM directors WHERE firstName = 'admin';

--Update Data
UPDATE flights
SET
    departureTime = 1100,
    arrivalTime = 1300,
    departureAirport = 'LAX',
    arrivalAirport = 'SFO',
    capacity = 100
WHERE
    flightNumber = 1;

UPDATE passengers
SET
    firstName = 'John',
    lastName = 'Doe',
    address = '123 Main St',
    phoneNumber = '123-456-7890',
    email = 'example@email.com',
    age = 21
WHERE
    passportNumber = 123456789;

UPDATE tickets
SET
    flightNumber = 1,
    passportNumber = 123456789,
    seatNumber = 'A1',
    price = 100.00
WHERE
    ticketNumber = 1;


UPDATE directors
SET
    firstName = 'admin',
    lastName = 'admin',
    address = '123 Main St',
    phoneNumber = '123-456-7890',
    email = 'example@email.com',
    age = 21
WHERE
    passportNumber = 123456789;
    
--Delete Data
DELETE FROM flights
WHERE
    flightNumber = 1;

DELETE FROM passengers
WHERE
    passportNumber = 123456789;

DELETE FROM tickets
WHERE
    ticketNumber = 1;

DELETE FROM directors
WHERE
    idNumber = 123456789;
