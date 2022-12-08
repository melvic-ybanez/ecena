//
// Created by Melvic Ybanez on 12/4/22.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "tuples.h"

class Ray {
public:
    rt::Point orig;
    rt::Vec dir;

    Ray();
    Ray(const rt::Point& origin, const rt::Vec &direction);

    rt::Point origin() const;
    rt::Vec direction() const;

    rt::Point at(double t) const;
};

#endif //RAYTRACER_RAY_H
