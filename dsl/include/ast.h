//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_AST_H
#define ECENA_AST_H

#include <string>
#include <vector>
#include "../../core/include/data.h"

namespace rt::dsl {
    class Field;

    enum class ExprType {
        object,
        number,
        string,
        array
    };

    std::string type_to_str(ExprType type);

    class Expr  {
    public:
        Expr() = default;

        virtual ~Expr() = default;

        Expr(const Expr &from) = delete;

        Expr(const Expr &&from) noexcept = delete;

        Expr &operator=(const Expr &from) = delete;

        Expr &operator=(Expr &&from) noexcept = delete;

        virtual ExprType type() const = 0;
    };

    class Object : public Expr {
    public:
        std::vector<Field> fields;

        explicit Object(std::vector<Field> fields);

        ExprType type() const override;
    };

    class String : public Expr {
    public:
        String(const std::string &value);

        bool operator==(const std::string &str) const;

        ExprType type() const override;

    private:
        std::string value;
    };

    class Number : public Expr {
    public:
        double value;

        Number(double value);

        ExprType type() const override;
    };

    class Array : public Expr {
    public:
        std::vector<std::unique_ptr<Expr>> elems;

        Array(std::vector<std::unique_ptr<Expr>> elems);

        ExprType type() const override;
    };

    class Field {
    public:
        int line;
        std::unique_ptr<String> key_;
        std::unique_ptr<Expr> value_;

        Field(std::unique_ptr<String> key, std::unique_ptr<Expr> value, int line);

        const String &key() const;

        const Expr &value() const;
    };
}

#endif //ECENA_AST_H
