//
// Created by Melvic Ybanez on 12/25/22.
//

#include <ostream>
#include "../include/errors.h"

namespace rt::dsl::errors {
    Error::Error(const std::string &message, int line) : std::runtime_error(message), line{line} {}

    std::ostream &operator<<(std::ostream &out, const Error &error) {
        return out << "An error occurred. " << error.what() << ". Line: " << error.line;
    }

    Error invalid_character(char c, int line) {
        return {"Invalid character: '" + std::string{c} + "'", line};
    }

    Error unterminated_string(int line) {
        return {"Unterminated string", line};
    }

    Error expected(const Token &token, const std::string &expected, const std::string &where) {
        return {"Expected: " + expected + " " + where + ". Got: " + token.lexeme, token.line};
    }

    Error type_mismatch(ExprType expected, ExprType got, int line) {
        return {"Type mismatch. Expected: " + type_to_str(expected) + ". Got: " + type_to_str(got), line};
    }

    Error wrong_args_count(size_t expected, size_t got, int line) {
        return {"Wrong number of arguments. Expected: " + std::to_string(expected) + ". Got: " + std::to_string(got), line};
    }
}