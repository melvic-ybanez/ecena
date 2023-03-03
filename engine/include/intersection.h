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
    public:
        real t;
        Shape* object;

        // Note: u and v are only needed for triangles. See if it's worth
        // moving these properties to a triangle-specific intersection subclass (probably not)
        real u;
        real v;

        Intersection(real t, Shape* object, real u = 0, real v = 0);

        bool operator<(const Intersection& that) const;

        bool operator==(const Intersection& that) const;
    };

    using AggregateData = std::vector<Intersection*>;

    class Aggregate {
    public:
        AggregateData elems;

        Aggregate(AggregateData elems);

        Aggregate() = default;

        Aggregate(const Aggregate& from) = default;

        Aggregate(Aggregate&& from) = default;

        Aggregate& operator=(const Aggregate& from) = default;

        Aggregate& operator=(Aggregate&& from) = default;

        ~Aggregate();

        size_t count() const;

        Intersection* operator[](size_t i);

        bool empty() const;

        Intersection* hit();

        void add(Intersection* intersection);

        void combine_with(Aggregate& that);

        void sort();

    private:
        bool is_sorted = false;
    };

    std::ostream& operator<<(std::ostream& out, const Intersection& intersection);
}

namespace rt {
    using Intersection = intersections::Intersection;
    using Aggregate = intersections::Aggregate;
}

#endif //ECENA_INTERSECTION_H
