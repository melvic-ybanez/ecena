//
// Created by Melvic Ybanez on 1/30/23.
//

#include <iostream>
#include "../include/interpreter.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/eval.h"

namespace rt::dsl {
    std::string read_source() {
        std::string source;
        std::string line;

        while (getline(std::cin, line)) {
            source += line + "\n";
        }

        return source;
    }

    std::optional<Data> interpret(const std::string &source) {
        std::unique_ptr<rt::dsl::Object> object;
        rt::Data data;
        Lexer lexer{source};

        try {
            auto tokens = lexer.scan_all();
            Parser parser{tokens};
            object = parser.parse_object();
            data = eval::to_data(*object);
        } catch (rt::dsl::errors::Error &error) {
            std::cout << error;
            return std::nullopt;
        }

        return data;
    }
}