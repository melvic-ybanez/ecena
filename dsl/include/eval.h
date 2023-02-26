//
// Created by Melvic Ybanez on 12/27/22.
//

#ifndef ECENA_EVAL_H
#define ECENA_EVAL_H

#include "../../engine/include/data.h"
#include "ast.h"

namespace rt::dsl::eval {
    Data to_data(const Object& object);

    std::unique_ptr<Shape> to_shape(World& world, const Expr& expr, int line);

    std::vector<std::unique_ptr<Shape>> to_shapes(World& world, const Expr& expr, int line);

    Material* to_material(World& world, shapes::Type shape_type, const Expr& expr, int line);

    PointLight to_point_light(const Expr& expr, int line);

    Camera to_camera(const Expr& expr, int line);

    World to_world(const Expr& expr, int line);

    std::unique_ptr<Pattern> to_pattern(shapes::Type shape_type, const Expr& expr, int line);
}

#endif //ECENA_EVAL_H
