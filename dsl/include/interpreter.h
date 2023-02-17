//
// Created by Melvic Ybanez on 1/30/23.
//

#ifndef ECENA_INTERPRETER_H
#define ECENA_INTERPRETER_H

#include "../../engine/include/data.h"

namespace rt::dsl {
    std::string read_source();

    std::optional<Data> interpret(const std::string& source);
}

#endif //ECENA_INTERPRETER_H
