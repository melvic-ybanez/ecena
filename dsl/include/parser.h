//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_PARSER_H
#define ECENA_PARSER_H

#include <vector>
#include <map>
#include "ast.h"
#include "tokens.h"

namespace rt::dsl::parsers {
    class Parser {
    public:
        std::unique_ptr<Expr> parse_expr();

        std::unique_ptr<Object> parse_object();

        std::unique_ptr<String> parse_string();

        std::optional<Token> match(TokenType type);

        bool check(TokenType type) const;

        bool is_at_end() const;

        const Token &peek() const;

        const Token &advance();

        const Token &previous() const;

        void consume(TokenType type, const std::string &expected, const std::string &after);

        std::vector<Field> parse_fields();

        Field parse_field();

    private:
        std::vector<Token> tokens;
        int current;
    };
}

#endif //ECENA_PARSER_H
