//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_AST_H
#define ECENA_AST_H

#include <string>
#include <vector>

namespace rt::dsl {
    class Field;

    class Expr {
    public:
        Expr();

        virtual ~Expr();

        Expr(const Expr &from) = delete;

        Expr(const Expr &&from) noexcept = delete;

        Expr &operator=(const Expr &from) = delete;

        Expr &operator=(Expr &&from) noexcept = delete;
    };

    class Object : public Expr {
    public:
        explicit Object(std::vector<Field> fields);

    private:
        std::vector<Field> fields;
    };

    class String : public Expr {
    public:
        String(const std::string &value);

    private:
        std::string value;
    };

    class Number : public Expr {
    public:
        Number(double value);

    private:
        double value;
    };

    class Array : public Expr {

    };

    class Field {
    public:
        Field(std::unique_ptr<String> key, std::unique_ptr<Expr> value);

    private:
        std::unique_ptr<String> key;
        std::unique_ptr<Expr> value;
    };
}

#endif //ECENA_AST_H
