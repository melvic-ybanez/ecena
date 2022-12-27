//
// Created by Melvic Ybanez on 12/27/22.
//

#ifndef ECENA_EVAL_H
#define ECENA_EVAL_H

#include "../../core/include/data.h"
#include "ast.h"

namespace rt::dsl::eval {
    Data to_data(const Object &object);

    Wall to_wall(const std::unique_ptr<Expr> &expr, int line);

    std::unique_ptr<Shape> to_shape(const std::unique_ptr<Expr> &expr, int line);

    std::vector<std::unique_ptr<Shape>> to_shapes(const std::unique_ptr<Expr> &expr, int line);
}

#endif //ECENA_EVAL_H
