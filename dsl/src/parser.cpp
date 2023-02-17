//
// Created by Melvic Ybanez on 12/25/22.
//

#include "../include/parser.h"
#include "../include/errors.h"

#define PARSE_EXPR(type, fn) \
    if (auto result = do_or_sync<type>([&] { return fn(); }); result != nullptr) return result

namespace rt::dsl {
    template<typename T, typename V, TokenType>
    static std::unique_ptr<T> parse_to_type(Parser& parser);

    Parser::Parser(const std::vector<Token> tokens) : tokens{tokens}, current{0} {}

    bool Parser::is_at_end() const {
        return peek().type == TokenType::eof;
    }

    const Token& Parser::peek() const {
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

    const Token& Parser::previous() const {
        return tokens[current - 1];
    }

    const Token& Parser::advance() {
        if (is_at_end()) return previous();
        current++;
        return previous();
    }

    Token Parser::consume(TokenType type, const std::string& expected, const std::string& where) {
        if (check(type)) {
            advance();
            return previous();
        } else {
            throw errors::expected(peek(), expected, where);
        }
    }

    std::unique_ptr<Expr> Parser::parse_expr() {
        PARSE_EXPR(String, parse_string);
        PARSE_EXPR(Boolean, parse_boolean);
        PARSE_EXPR(Number, parse_number);
        PARSE_EXPR(Array, parse_array);
        PARSE_EXPR(Null, parse_null);
        return parse_object();
    }

    std::unique_ptr<String> Parser::parse_string() {
        return parse_to_type<String, std::string, TokenType::string>(*this);
    }

    std::unique_ptr<Object> Parser::parse_object() {
        consume(TokenType::left_brace, "{", "at the start of an object expression");
        std::vector<Field> fields = parse_fields();
        consume(TokenType::right_brace, "}", "after all the fields");
        return std::make_unique<Object>(std::move(fields));
    }

    std::unique_ptr<Boolean> Parser::parse_boolean() {
        return parse_to_type<Boolean, bool, TokenType::boolean>(*this);
    }

    std::unique_ptr<Number> Parser::parse_number() {
        return parse_to_type<Number, double, TokenType::number>(*this);
    }

    std::unique_ptr<Null> Parser::parse_null() {
        if (auto result = match(TokenType::null); result.has_value()) {
            return std::make_unique<Null>();
        }
        return nullptr;
    }

    template<typename T, typename V, TokenType TT>
    std::unique_ptr<T> parse_to_type(Parser& parser) {
        if (auto result = parser.match(TT); result.has_value()) {
            return std::make_unique<T>(std::get<V>(result.value().value.value()));
        }
        return nullptr;
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