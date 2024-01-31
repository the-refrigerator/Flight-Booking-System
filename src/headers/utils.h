#pragma once

#include <string>

using namespace std;

class Utils {
public:
    static bool verify_email(string email);
    static bool verify_phone_number (string phone_number);
    static bool validate_datetime(string datetime);
};
