//
// Created by Melvic Ybanez on 12/25/22.
//

#include <ostream>
#include "../include/errors.h"

namespace rt::dsl::errors {
    Error::Error(const std::string &message, int line) : std::runtime_error(message), line{line} {}

    std::ostream &operator<<(std::ostream &out, const Error &error) {
        return out << "An error occurred prior to rendering. " << error.what() << ". Line: " << error.line;
    }

    Error invalid_character(char c, int line) {
        return {"Invalid character: '" + std::string{c} + "'", line};
    }

    Error unterminated_string(int line) {
        return {"Unterminated string", line};
    }

    Error expected(const Token &token, const std::string &expected, const std::string &where) {
        return {"Expected: '" + expected + "' " + where + ". Got: '" + token.lexeme + "'", token.line};
    }

    Error type_mismatch(ExprType expected, ExprType got, int line) {
        return type_mismatch(type_to_str(expected), type_to_str(got), line);
    }

    Error type_mismatch(const std::string &expected, const std::string &got, int line) {
        return {"Type mismatch. Expected: " + expected + ". Got: " + got, line};
    }

    Error wrong_args_count(size_t expected, size_t got, int line) {
        return {"Wrong number of arguments. Expected: " + std::to_string(expected) + ". Got: " + std::to_string(got),
                line};
    }

    Error unknown_field(const std::string &field_name, int line) {
        return {"Unknown field: " + field_name, line};
    }

    Error invalid_kind(const std::string &kind, const std::string &object, int line) {
        return {"Invalid type '" + kind + "' for object '" + object + "'", line};
    }

    Error required_type(const std::string &type, int line) {
        return {"Required type: '" + type + "'", line};
    }

    Error invalid_path(const std::string &path, int line) {
        return {"Invalid path: '" + path + "'", line};
    }

    Error obj_not_found(int line) {
        return {"No OBJ model provided", line};
    }
}