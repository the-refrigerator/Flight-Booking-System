#ifndef UTILS_HPP
#define UTILS_HPP

#include <regex>
#include <string>

using namespace std;

class Utils {
public:
    static bool verify_email(string email) {
        regex pattern("^(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+$");
        return regex_match(email, pattern);
    }
};

#endif //!UTILS_HPP
