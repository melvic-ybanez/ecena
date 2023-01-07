//
// Created by Melvic Ybanez on 1/1/23.
//

#ifndef ECENA_TEST_UTILS_H
#define ECENA_TEST_UTILS_H

#include "../../engine/include/world.h"

namespace rt::tests {
    World default_world();

    class TestShape : public Shape {
    public:
        Ray saved_ray;

        rt::shapes::Type type() const override;

        Aggregate local_intersect(const Ray &local_ray) override;

        Vec local_normal_at(const Point &local_point) override;
    };

    class TestPattern : public Pattern {
    public:
        using Pattern::at;

        Color at(const Point &point) const override;
    };
}

#endif //ECENA_TEST_UTILS_H
