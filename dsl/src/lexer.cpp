//
// Created by Melvic Ybanez on 12/25/22.
//

#include <utility>

#include "../include/lexer.h"
#include "../include/errors.h"

namespace rt::dsl {
    Lexer::Lexer() {
        start = 0;
        current = 0;
        line = 1;
    }

    Lexer::Lexer(const std::string &source) : Lexer() {
        this->source = source;
    }

    std::vector<Token> Lexer::scan_all() {
        while (!is_at_end()) {
            scan_next();
        }
        tokens.push_back({TokenType::eof, "", line});
        return tokens;
    }

    void Lexer::scan() {
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
            case '-':
            case '+':
                scan_digit();
                break;
            default:
                if (std::isdigit(c)) {
                    scan_digit();
                } else if (std::isalpha(c)) {
                    scan_identifier();
                } else {
                    throw errors::invalid_character(c, line);
                }
        }
    }

    void Lexer::scan_next() {
        start = current;
        scan();
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
        add_token(type, {});
    }

    void Lexer::add_token(TokenType type, TokenValue value) {
        tokens.push_back({type, lexeme(), line, std::move(value)});
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
            auto content = source.substr(start + 1, current - start - 2);
            add_token(TokenType::string, content);
        }
    }

    void Lexer::scan_digit() {
        while (std::isdigit(peek()) && !is_at_end()) advance();
        if (peek() == '.' && std::isdigit(peek_next())) {
            advance();
            while (std::isdigit(peek())) advance();
        }
        auto value_str = source.substr(start, current - start);
        auto value = std::stod(value_str);
        add_token(TokenType::number, value);
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

    void Lexer::scan_identifier() {
        while (std::isalpha(peek()) && !is_at_end()) advance();
        auto value_str = source.substr(start, current - start);
        if (value_str == "true" || value_str == "false") {
            add_token(TokenType::boolean, value_str == "true");
        } else if (value_str == "null") {
            add_token(TokenType::null);
        }
    }
}