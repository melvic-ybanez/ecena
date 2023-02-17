//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_PARSER_H
#define ECENA_PARSER_H

#include <vector>
#include <map>
#include "ast.h"
#include "tokens.h"
#include "errors.h"

namespace rt::dsl {
    class Parser {
    public:
        Parser(const std::vector<Token> tokens);

        std::unique_ptr<Expr> parse_expr();

        std::unique_ptr<Object> parse_object();

        std::unique_ptr<String> parse_string();

        std::unique_ptr<Array> parse_array();

        std::unique_ptr<Boolean> parse_boolean();

        std::unique_ptr<Number> parse_number();

        std::unique_ptr<Null> parse_null();

        std::optional<Token> match(TokenType type);

        bool check(TokenType type) const;

        bool is_at_end() const;

        const Token& peek() const;

        const Token& advance();

        const Token& previous() const;

        Token consume(TokenType type, const std::string& expected, const std::string& where);

        std::vector<Field> parse_fields();

        Field parse_field();

        template<typename T>
        std::unique_ptr<T> do_or_sync(std::function<std::unique_ptr<T>()> f) {
            int current_ = current;
            try {
                return f();
            } catch (errors::Error& error) {
                current = current_;
                return nullptr;
            }
        }

    private:
        std::vector<Token> tokens;
        int current;
    };
}

#endif //ECENA_PARSER_H
