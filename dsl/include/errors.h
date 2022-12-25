//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_ERRORS_H
#define ECENA_ERRORS_H

#include <string>
#include "tokens.h"

namespace rt::dsl::errors {
    class Error : public std::runtime_error {
    public:
        int line;

        Error(const std::string &message, int line);
    };

    std::ostream &operator<<(std::ostream &out, const Error &error);

    Error invalid_character(char c, int line);

    Error unterminated_string(int line);

    Error expected(const Token &token, const std::string &expected, const std::string &where);
}

#endif //ECENA_ERRORS_H
