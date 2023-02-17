//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_LEXER_H
#define ECENA_LEXER_H

#include <vector>
#include "tokens.h"

namespace rt::dsl {
    class Lexer {
    public:
        std::string source;
        int start;
        int current;
        int line;
        std::vector<Token> tokens;

        Lexer();

        Lexer(const std::string& source);

        std::vector<Token> scan_all();

        void scan();

        void scan_next();

        void advance();

        char read_and_advance();

        bool is_at_end() const;

        char peek() const;

        char peek_next() const;

    private:
        void add_token(TokenType type);

        void add_token(TokenType type, TokenValue value);

        void next_line();

        void scan_string();

        void scan_digit();

        void scan_identifier();

        std::string lexeme() const;
    };
}

#endif //ECENA_LEXER_H
