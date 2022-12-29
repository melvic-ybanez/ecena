//
// Created by Melvic Ybanez on 12/25/22.
//

#include "../include/parser.h"
#include "../include/errors.h"

namespace rt::dsl {
    Parser::Parser(const std::vector<Token> tokens) : tokens{tokens}, current{0} {}

    bool Parser::is_at_end() const {
        return peek().type == TokenType::eof;
    }

    const Token &Parser::peek() const {
        return tokens[current];
    }

    std::optional<Token> Parser::match(TokenType type) {
        if (check(type)) {
            advance();
            return {previous()};
        }
        return {};
    }

    bool Parser::check(TokenType type) const {
        return !is_at_end() && peek().type == type;
    }

    const Token &Parser::previous() const {
        return tokens[current - 1];
    }

    const Token &Parser::advance() {
        if (is_at_end()) return previous();
        current++;
        return previous();
    }

    Token Parser::consume(TokenType type, const std::string &expected, const std::string &where) {
        if (check(type)) {
            advance();
            return previous();
        } else {
            throw errors::expected(peek(), expected, where);
        }
    }

    std::unique_ptr<Expr> Parser::parse_expr() {
        if (auto result = do_or_sync<Object>([&] { return parse_object(); }); result != nullptr) return result;
        if (auto result = do_or_sync<String>([&] { return parse_string(); }); result != nullptr) return result;
        auto result = do_or_sync<Number>([&] {
            std::unique_ptr<Number> result;
            if (auto result_ = match(TokenType::number); result_.has_value()) {
                result = std::make_unique<Number>(std::get<double>(result_.value().value.value()));
            }
            return result;
        });
        if (result != nullptr) return result;
        return parse_array();
    }

    std::unique_ptr<String> Parser::parse_string() {
        if (auto result = match(TokenType::string); result.has_value()) {
            return std::make_unique<String>(std::get<std::string>(result.value().value.value()));
        }
        return nullptr;
    }

    std::unique_ptr<Object> Parser::parse_object() {
        consume(TokenType::left_brace, "{", "at the start of source");
        std::vector<Field> fields = parse_fields();
        consume(TokenType::right_brace, "}", "after all the fields");
        return std::make_unique<Object>(std::move(fields));
    }

    Field Parser::parse_field() {
        auto key = parse_string();
        auto colon = consume(TokenType::colon, ":", "after field key");
        auto value = parse_expr();
        return {std::move(key), std::move(value), colon.line};
    }

    std::vector<Field> Parser::parse_fields() {
        std::vector<Field> fields;
        while (peek().type == TokenType::string) {
            fields.push_back(parse_field());
            if (peek().type != TokenType::right_brace)
                consume(TokenType::comma, ",", "after a field");
        }
        return fields;
    }

    std::unique_ptr<Array> Parser::parse_array() {
        consume(TokenType::left_bracket, "[", "at the start of array expression");
        std::vector<std::unique_ptr<Expr>> elems;

        // first, check if the array is empty, so we don't make the mistake of doing the loop
        if (peek().type == TokenType::right_bracket) {
            advance();
            return std::make_unique<Array>(std::move(elems));
        }

        // now we can assume the array has some elements. Parse the first one since it's
        // a special case (not preceded by a comma)
        auto first = parse_expr();
        elems.push_back(std::move(first));

        while (peek().type == TokenType::comma) {
            consume(TokenType::comma, ",", "after array element");
            auto elem = parse_expr();
            elems.push_back(std::move(elem));
        }

        consume(TokenType::right_bracket, "]", "at the end of array expression");

        return std::make_unique<Array>(std::move(elems));
    }
}