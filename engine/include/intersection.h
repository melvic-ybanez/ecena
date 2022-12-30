//
// Created by Melvic Ybanez on 12/18/22.
//

#ifndef ECENA_INTERSECTION_H
#define ECENA_INTERSECTION_H

#include <vector>
#include "../math/include/math_utils.h"

namespace rt::shapes {
    class Shape;
}

namespace rt {
    using Shape = shapes::Shape;
}

namespace rt::intersections {
    class Intersection {
        real t_;
        const Shape *shape_;

    public:
        Intersection(real t, const Shape *shape);

        real t() const;

        const Shape *shape() const;

        bool operator<(const Intersection &that) const;
    };

    using AggregateData = std::vector<Intersection *>;

    class Aggregate {
    public:
        AggregateData elems;

        Aggregate(AggregateData elems_);

        Aggregate() = default;

        size_t count() const;

        Intersection *operator[](size_t i);

        bool empty() const;

        Intersection *hit();

        void combine_with(const Aggregate &agg);

        void sort();

    private:
        bool is_sorted;
    };
}

namespace rt {
    using Intersection = intersections::Intersection;
}

#endif //ECENA_INTERSECTION_H
