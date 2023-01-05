//
// Created by Melvic Ybanez on 1/5/23.
//

#ifndef ECENA_PATTERNS_H
#define ECENA_PATTERNS_H

#include "../math/include/tuples.h"

namespace rt::patterns {
    class Pattern {
    public:
        Pattern() = default;

        Pattern(const Pattern &from) = delete;

        Pattern(Pattern &&from) = delete;

        Pattern &operator=(const Pattern &from) = delete;

        Pattern &operator=(Pattern &&from) = delete;

        virtual ~Pattern() = default;

        virtual Color at(const Point &point) const = 0;
    };

    class Stripe : public Pattern {
    public:
        Color first;
        Color second;

        Stripe(Color first, Color second);

        Color at(const Point &point) const override;
    };
}

namespace rt {
    using Pattern = patterns::Pattern;
}

#endif //ECENA_PATTERNS_H
