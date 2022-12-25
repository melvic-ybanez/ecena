//
// Created by Melvic Ybanez on 12/26/22.
//

#include "../include/ast.h"

namespace rt::dsl {
    Expr::Expr() = default;

    Expr::~Expr() = default;

    Object::Object(std::vector<Field> fields) : fields{std::move(fields)} {}

    String::String(const std::string &value) : value{value} {}

    Number::Number(double value) : value{value} {}

    Field::Field(std::unique_ptr<String> key, std::unique_ptr<Expr> value) :
            key{std::move(key)},
            value{std::move(value)} {}

    Array::Array(std::vector<std::unique_ptr<Expr>> elems) : elems{std::move(elems)} {}
}