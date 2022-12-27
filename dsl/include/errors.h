//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_ERRORS_H
#define ECENA_ERRORS_H

#include <string>
#include "tokens.h"
#include "ast.h"

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

    Error type_mismatch(ExprType expected, ExprType got, int line);

    Error type_mismatch(const std::string &expected, const std::string &got, int line);

    Error wrong_args_count(size_t expected, size_t got, int line);
}

#endif //ECENA_ERRORS_H
