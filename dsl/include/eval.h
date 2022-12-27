//
// Created by Melvic Ybanez on 12/27/22.
//

#ifndef ECENA_EVAL_H
#define ECENA_EVAL_H

#include "../../core/include/data.h"
#include "ast.h"

namespace rt::dsl::eval {
    Data object(const Object &object);

    void to_ray_origin(const Field &field, Data &data);
}

#endif //ECENA_EVAL_H
