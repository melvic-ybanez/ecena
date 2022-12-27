//
// Created by Melvic Ybanez on 12/26/22.
//

#include "../include/ast.h"
#include "../include/errors.h"

namespace rt::dsl {
    std::ostream &operator<<(std::ostream &out, const Expr &expr) {
        return expr.display(out);
    }

    std::string type_to_str(ExprType type) {
        switch(type) {
            case ExprType::object: return "Object";
            case ExprType::number: return "Number";
            case ExprType::string: return "String";
            case ExprType::array: return "Array";
        }
    }

    Object::Object(std::vector<Field> fields) : fields{std::move(fields)} {}

    ExprType Object::type() const {
        return ExprType::object;
    }

    std::ostream &Object::display(std::ostream &out) const {
        out << "{ ";
        size_t r = 0;
        for (auto &field: fields) {
            out << field;
            r++;
            if (r != fields.size()) out << ", ";
        }
        return out << " }";
    }

    String::String(const std::string &value) : value{value} {}

    ExprType String::type() const {
        return ExprType::string;
    }

    bool String::operator==(const std::string &str) const {
        return value == str;
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
        out << "[ ";
        size_t r = 0;
        for (auto &elem : elems) {
            out << elem;
            r++;
            if (r != elems.size()) out << ", ";
        }
        return out << " ]";
    }
}