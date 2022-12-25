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

    Error expected(const Token &token, const std::string &expected, const std::string &after) {
        return {"Expected: " + expected + " after " + after + ". Got: " + token.lexeme, token.line};
    }

    Error invalid_expression(const Token &token) {
        return {"Invalid Expression: " + token.lexeme, token.line};
    }
}