//
// Created by Melvic Ybanez on 12/25/22.
//

#include "../include/parser.h"
#include "../include/errors.h"

namespace rt::dsl::parsers {
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

    void Parser::consume(TokenType type, const std::string &expected, const std::string &after) {
        if (check(type)) {
            advance();
        } else {
            throw errors::expected(peek(), expected, after);
        }
    }

    std::unique_ptr<Expr> Parser::parse_expr() {
        if (auto result = parse_object(); result != nullptr) return result;
        if (auto result = parse_string(); result != nullptr) return result;
        if (auto result = match(TokenType::number); result.has_value()) {
            return std::make_unique<Number>(std::get<double>(result.value().value.value()));
        }
        // TODO: Parse array here
        throw errors::invalid_expression(peek());
    }

    std::unique_ptr<String> Parser::parse_string() {
        if (auto result = match(TokenType::string); result.has_value()) {
            return std::make_unique<String>(std::get<std::string>(result.value().value.value()));
        }
        return nullptr;
    }

    std::unique_ptr<Object> Parser::parse_object() {
        consume(TokenType::left_brace, "{", " start of source");
        std::vector<Field> fields = parse_fields();
        consume(TokenType::right_brace, "}", "object fields");
        return std::make_unique<Object>(std::move(fields));
    }

    Field Parser::parse_field() {
        auto key = parse_string();
        consume(TokenType::colon, ":", "field key");
        auto value = parse_expr();
        return {std::move(key), std::move(value)};
    }

    std::vector<Field> Parser::parse_fields() {
        std::vector<Field> fields;
        while (peek().type == TokenType::string) {
            fields.push_back(parse_field());
        }
        return fields;
    }
}