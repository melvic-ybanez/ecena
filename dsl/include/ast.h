//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_AST_H
#define ECENA_AST_H

#include <vector>
#include "../../engine/include/data.h"

namespace rt::dsl {
    class Field;

    enum class ExprType {
        object, number, string, array, boolean, null
    };

    std::string type_to_str(ExprType type);

    class Expr {
    public:
        Expr() = default;

        virtual ~Expr() = default;

        Expr(const Expr& from) = delete;

        Expr(const Expr&& from) noexcept = delete;

        Expr& operator=(const Expr& from) = delete;

        Expr& operator=(Expr&& from) noexcept = delete;

        virtual ExprType type() const = 0;

        virtual std::ostream& display(std::ostream& out) const = 0;
    };

    class Object : public Expr {
    public:
        std::vector<Field> fields;

        explicit Object(std::vector<Field> fields);

        [[nodiscard]] ExprType type() const override;

        std::ostream& display(std::ostream& out) const override;
    };

    class String : public Expr {
    public:
        std::string value;

        String(const std::string& value);

        bool operator==(const std::string& str) const;

        bool operator!=(const std::string& str) const;

        [[nodiscard]] ExprType type() const override;

        std::ostream& display(std::ostream& out) const override;
    };

    class Number : public Expr {
    public:
        double value;

        explicit Number(double value);

        [[nodiscard]] ExprType type() const override;

        std::ostream& display(std::ostream& out) const override;
    };

    class Array : public Expr {
    public:
        std::vector<std::unique_ptr<Expr>> elems;

        explicit Array(std::vector<std::unique_ptr<Expr>> elems);

        [[nodiscard]] ExprType type() const override;

        std::ostream& display(std::ostream& out) const override;
    };

    class Boolean : public Expr {
    public:
        bool value;

        explicit Boolean(bool value);

        [[nodiscard]] ExprType type() const override;

        std::ostream& display(std::ostream& out) const override;
    };

    class Null : public Expr {
    public:
        [[nodiscard]] ExprType type() const override;

        std::ostream& display(std::ostream& out) const override;
    };

    class Field {
    public:
        std::unique_ptr<String> key_;
        std::unique_ptr<Expr> value_;
        int line;

        Field(std::unique_ptr<String> key, std::unique_ptr<Expr> value, int line);

        [[nodiscard]] const String& key() const;

        [[nodiscard]] const Expr& value() const;
    };

    std::ostream& operator<<(std::ostream& out, const Expr& expr);

    std::ostream& operator<<(std::ostream& out, const Field& field);
}

#endif //ECENA_AST_H
