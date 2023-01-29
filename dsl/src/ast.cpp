//
// Created by Melvic Ybanez on 12/26/22.
//

#include "../include/ast.h"
#include "../include/errors.h"
#include "../../shared/include/utils.h"

namespace rt::dsl {
    std::ostream &operator<<(std::ostream &out, const Expr &expr) {
        return expr.display(out);
    }

    std::string type_to_str(ExprType type) {
        std::array<std::string, 6> type_strings{
            "Object", "Number", "String", "Array", "Boolean", "Null"
        };
        return type_strings[static_cast<int>(type)];
    }

    Object::Object(std::vector<Field> fields) : fields{std::move(fields)} {}

    ExprType Object::type() const {
        return ExprType::object;
    }

    std::ostream &Object::display(std::ostream &out) const {
        return out << join_to_object(fields);
    }

    String::String(const std::string &value) : value{value} {}

    ExprType String::type() const {
        return ExprType::string;
    }

    bool String::operator==(const std::string &str) const {
        return value == str;
    }

    bool String::operator!=(const std::string &str) const {
        return !(*this == str);
    }

    std::ostream &String::display(std::ostream &out) const {
        return out << "\"" << value << "\"";
    }

    Number::Number(double value) : value{value} {}

    std::ostream &Number::display(std::ostream &out) const {
        return out << value;
    }

    ExprType Number::type() const {
        return ExprType::number;
    }

    Boolean::Boolean(bool value) : value{value} {}

    ExprType Boolean::type() const {
        return ExprType::boolean;
    }

    std::ostream &Boolean::display(std::ostream &out) const {
        return out << value;
    }

    Field::Field(std::unique_ptr<String> key, std::unique_ptr<Expr> value, int line) :
            key_{std::move(key)},
            value_{std::move(value)},
            line{line} {}

    const String &Field::key() const {
        return *key_;
    }

    const Expr &Field::value() const {
        return *value_;
    }

    std::ostream &operator<<(std::ostream &out, const Field &field) {
        return out << field.key() << ": " << field.value();
    }

    Array::Array(std::vector<std::unique_ptr<Expr>> elems) : elems{std::move(elems)} {}

    ExprType Array::type() const {
        return ExprType::array;
    }

    std::ostream &Array::display(std::ostream &out) const {
        return out << join_to_array(elems);
    }

    ExprType Null::type() const {
        return ExprType::null;
    }

    std::ostream &Null::display(std::ostream &out) const {
        return out << "null";
    }
}