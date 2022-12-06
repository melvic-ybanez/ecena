//
// Created by Melvic Ybanez on 12/4/22.
//

#include "../include/ray.h"

Ray::Ray() = default;

Ray::Ray(const tuples::Point &origin, const tuples::Vec &direction) : orig(origin), dir(direction) {}

tuples::Point Ray::origin() const { return orig; }

tuples::Vec Ray::direction() const { return dir; }

tuples::Point Ray::at(double t) const {
    return orig + t * dir;
}
