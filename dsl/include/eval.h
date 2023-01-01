//
// Created by Melvic Ybanez on 12/27/22.
//

#ifndef ECENA_EVAL_H
#define ECENA_EVAL_H

#include "../../engine/include/data.h"
#include "ast.h"
#include "../../engine/include/light.h"

namespace rt::dsl::eval {
    Data to_data(const Object &object);

    std::unique_ptr<Shape> to_shape(const std::unique_ptr<Expr> &expr, int line);

    std::vector<std::unique_ptr<Shape>> to_shapes(const std::unique_ptr<Expr> &expr, int line);

    Material to_material(const std::unique_ptr<Expr> &expr, int line);

    PointLight to_point_light(const std::unique_ptr<Expr> &expr, int line);

    Camera to_camera(const std::unique_ptr<Expr> &expr, int line);

    World to_world(const std::unique_ptr<Expr> &expr, int line);
}

#endif //ECENA_EVAL_H
