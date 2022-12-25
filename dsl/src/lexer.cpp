//
// Created by Melvic Ybanez on 12/25/22.
//

#include "../include/lexer.h"
#include "../include/errors.h"

namespace rt::dsl::lexer {
    Lexer::Lexer() {
        start = -1;
        current = -1;
    }

    std::vector<Token> Lexer::scan() {
        auto c = read_and_advance();
        switch (c) {
            case '{':
                add_token(TokenType::left_brace);
                break;
            case '}':
                add_token(TokenType::right_brace);
                break;
            case ',':
                add_token(TokenType::comma);
                break;
            case '[':
                add_token(TokenType::left_bracket);
                break;
            case ']':
                add_token(TokenType::right_bracket);
                break;
            case ':':
                add_token(TokenType::colon);
                break;
            case '.':
                add_token(TokenType::dot);
                break;
            case '-':
                add_token(TokenType::minus);
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                next_line();
                break;
            case '"':
                scan_string();
                break;
            default:
                if (std::isdigit(c)) {
                    scan_digit();
                } else {
                    throw errors::invalid_character(c, line);
                }
        }
        return tokens;
    }

    char Lexer::read_and_advance() {
        auto c = source[current];
        advance();
        return c;
    }

    void Lexer::advance() {
        current++;
    }

    void Lexer::add_token(TokenType type) {
        add_token(type, lexeme());
    }

    void Lexer::add_token(TokenType type, const std::string &lexeme) {
        tokens.push_back({ type, lexeme, line });
    }

    void Lexer::next_line() {
        line++;
    }

    void Lexer::scan_string() {
        while (peek() != '"' && !is_at_end()) {
            advance();
        }

        if (is_at_end()) {
            throw errors::unterminated_string(line);
        } else {
            advance(); // remove the closing quotation
            auto content = source.substr(start + 1, current - 1);
            add_token(TokenType::string, content);
        }
    }

    void Lexer::scan_digit() {
        while (std::isdigit(peek())) advance();
        if (peek() == '.' && std::isdigit(peek_next())) {
            advance();
            while (std::isdigit(peek())) advance();
        }
        add_token(TokenType::number);
    }

    bool Lexer::is_at_end() const {
        return current >= source.size();
    }

    std::string Lexer::lexeme() const {
        return source.substr(start, current);
    }

    char Lexer::peek() const {
        if (is_at_end()) return 0;
        return source[current];
    }

    char Lexer::peek_next() const {
        if (current + 1 >= source.size()) return 0;
        return source[current + 1];
    }
}