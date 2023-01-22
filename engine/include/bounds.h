//
// Created by Melvic Ybanez on 1/22/23.
//

#ifndef ECENA_BOUNDS_H
#define ECENA_BOUNDS_H

#include "../math/include/tuples.h"
#include "../math/include/matrix.h"

namespace rt {
    class Bounds {
    public:
        Point max;
        Point min;

        static Bounds cube();

        /**
         * Creates a box that has no space. This box is considered invalid.
         */
        Bounds();

        Bounds(Point min, Point max);

        /**
         * Creates a bounding box with opposing minium and maximum
         */
        Bounds(const Point &min);

        Bounds transform(const Matrix<4, 4> &transformation) const;

        Bounds operator+(const Bounds &other) const;

        Bounds operator+(const Point &point) const;
    };

    std::ostream &operator<<(std::ostream &out, const Bounds &bounds);
}

#endif //ECENA_BOUNDS_H
