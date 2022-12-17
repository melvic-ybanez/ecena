//
// Created by Melvic Ybanez on 12/17/22.
//

#include "../include/ray.h"

namespace rt {
    Ray::Ray(const Point &origin, const Vec &direction): origin_{origin}, direction_{direction} {}

    const Point &Ray::origin() const {
        return origin_;
    }

    const Vec &Ray::direction() const {
        return direction_;
    }
}
