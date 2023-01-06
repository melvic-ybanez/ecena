//
// Created by Melvic Ybanez on 1/5/23.
//

#ifndef ECENA_PATTERNS_H
#define ECENA_PATTERNS_H

#include "../math/include/tuples.h"
#include "../math/include/matrix.h"


namespace rt::shapes {
    class Shape;
}

namespace rt {
    using Shape = shapes::Shape;
}

namespace rt::patterns {
    class Pattern {
    public:
        Matrix<4, 4> transformation;

        Pattern();

        Pattern(const Pattern &from) = delete;

        Pattern(Pattern &&from) = delete;

        Pattern &operator=(const Pattern &from) = delete;

        Pattern &operator=(Pattern &&from) = delete;

        virtual ~Pattern() = default;

        virtual Color at(const Point &point) const = 0;

        virtual Color at(const Shape &shape, const Point &world_point) const = 0;
    };

    class Stripe : public Pattern {
    public:
        Color first;
        Color second;

        Stripe(Color first, Color second);

        Color at(const Point &point) const override;

        Color at(const Shape &shape, const Point &world_point) const override;
    };
}

namespace rt {
    using Pattern = patterns::Pattern;
}

#endif //ECENA_PATTERNS_H
