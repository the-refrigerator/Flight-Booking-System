#include <regex>
#include <string>
#include "../headers/utils.h"

using namespace std;

// Please note that the `inline` keyword is not necessary here, but it is a recommendations by clang.

bool Utils::verify_email(string email) {
    regex pattern("^(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+$");
    return regex_match(email, pattern);
}

bool Utils::verify_phone_number (string phone_number) {
    regex pattern("(?:([+]\\d{1,4})[-.\\s]?)?(?:[(](\\d{1,3})[)][-.\\s]?)?(\\d{1,4})[-.\\s]?(\\d{1,4})[-.\\s]?(\\d{1,9})");
    return regex_match(phone_number, pattern);
}

bool Utils::validate_datetime(string datetime) {
    regex pattern("^(0[1-9]|1\\d|2[0-8]|29(?=-\\d\\d-(?!1[01345789]00|2[1235679]00)\\d\\d(?:[02468][048]|[13579][26]))|30(?!-02)|31(?=-0[13578]|-1[02]))-(0[1-9]|1[0-2])-([12]\\d{3}) ([0]\\d|1[0-2]):([0-5]\\d):([0-5]\\d) (AM|PM)$");
    return regex_match(datetime, pattern);
}
