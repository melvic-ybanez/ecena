//
// Created by Melvic Ybanez on 12/17/22.
//

#include <iostream>
#include "../include/ray.h"

namespace rt {
    Ray::Ray(const Point &origin, const Vec &direction): origin_{origin}, direction_{direction} {}

    Ray::Ray(Point &&origin, Vec &&direction): origin_{std::move(origin)}, direction_{std::move(direction)} {}

    const Point &Ray::origin() const {
        return origin_;
    }

    const Vec &Ray::direction() const {
        return direction_;
    }

    Point Ray::at(real t) const {
        return rt::Point{origin() + direction() * t};
    }

    std::ostream &operator<<(std::ostream &out, const Ray &ray) {
        return out << "origin: " << ray.origin() << "; direction " << ray.direction();
    }
}
