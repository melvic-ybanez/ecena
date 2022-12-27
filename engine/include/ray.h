//
// Created by Melvic Ybanez on 12/17/22.
//

#ifndef ECENA_RAY_H
#define ECENA_RAY_H

#include "../math/include/tuples.h"
#include "../math/include/matrix.h"

namespace rt {
    class Ray {
        Point origin_;
        Vec direction_;

    public:
        Ray(const Point &origin, const Vec &direction);

        Ray(Point &&origin, Vec &&direction);

        Ray(Tuple &&origin, Tuple &&direction);

        const Point &origin() const;

        const Vec &direction() const;

        Point at(real t) const;

        Ray transform(const math::Matrix<4, 4> &matrix) const;

        Ray translate(real x, real y, real z) const;

        Ray scale(real x, real y, real z) const;

        Ray rotate_x(real r) const;

        Ray rotate_y(real r) const;

        Ray rotate_z(real r) const;

        Ray shear(real xy, real xz, real yx, real yz, real zx, real zy) const;

        bool operator==(const Ray &that) const;
    };

    std::ostream &operator<<(std::ostream &out, const Ray &ray);
}

#endif //ECENA_RAY_H
