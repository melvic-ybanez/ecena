//
// Created by Melvic Ybanez on 12/4/22.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "tuple.h"

class Ray {
public:
    tuples::Point orig;
    tuples::Vec dir;

    Ray();
    Ray(const tuples::Point& origin, const tuples::Vec &direction);

    tuples::Point origin() const;
    tuples::Vec direction() const;

    tuples::Point at(double t) const;
};

#endif //RAYTRACER_RAY_H
