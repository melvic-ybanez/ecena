//
// Created by Melvic Ybanez on 12/4/22.
//

#include "../include/ray.h"

Ray::Ray() = default;

Ray::Ray(const rt::Point &origin, const rt::Vec &direction) : orig(origin), dir(direction) {}

rt::Point Ray::origin() const { return orig; }

rt::Vec Ray::direction() const { return dir; }

rt::Point Ray::at(double t) const {
    return orig + t * dir;
}
