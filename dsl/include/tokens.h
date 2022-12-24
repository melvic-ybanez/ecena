//
// Created by Melvic Ybanez on 12/25/22.
//

#ifndef ECENA_TOKENS_H
#define ECENA_TOKENS_H

#include <string>

namespace rt::dsl {
    enum class TokenType {
        number,
        string,
        left_brace,
        right_brace,
        left_bracket,
        right_bracket,
        comma,
        colon,
        dot,
        minus
    };

    struct Token {
        TokenType type;
        std::string lexeme;
        int line;
    };
}

#endif //ECENA_TOKENS_H
