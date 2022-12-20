//
// Created by Melvic Ybanez on 12/18/22.
//

#ifndef ECENA_INTERSECTION_H
#define ECENA_INTERSECTION_H

#include <vector>
#include "../../math/include/math_utils.h"

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
    };

    using SetElems = std::vector<Intersection *>;

    class Set {
        SetElems elems_;

    public:
        Set(SetElems elems_);

        Set();

        Set(const Set &set);

        Set(Set &&set) noexcept;

        Set &operator=(const Set &other);

        Set &operator=(Set &&other) noexcept;

        ~Set();

        size_t size() const;

        Intersection *operator[](size_t i);

        bool empty() const;
    };
}

namespace rt {
    using Intersection = intersections::Intersection;
}

#endif //ECENA_INTERSECTION_H
