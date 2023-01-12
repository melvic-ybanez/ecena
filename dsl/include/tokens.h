//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_TOKENS_H
#define ECENA_TOKENS_H

#include <optional>
#include <string>

namespace rt::dsl {
    enum class TokenType {
        number,
        string,
        boolean,
        left_brace,
        right_brace,
        left_bracket,
        right_bracket,
        comma,
        colon,
        dot,
        eof,
        null
    };

    using TokenValue = std::optional<std::variant<std::string, double, bool>>;

    struct Token {
        TokenType type;
        std::string lexeme;
        int line;
        TokenValue value;
    };
}

#endif //ECENA_TOKENS_H
