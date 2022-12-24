//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_ERRORS_H
#define ECENA_ERRORS_H

#include <string>

namespace rt::dsl::errors {
    struct Error : std::runtime_error {
        int line;

        Error(const std::string &message, int line);
    };

    std::ostream &operator<<(std::ostream &out, const Error &error);

    Error invalid_character(char c, int line);

    Error unterminated_string(int line);
}

#endif //ECENA_ERRORS_H
