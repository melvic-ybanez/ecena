//
// Created by Melvic Ybanez on 12/31/22.
//

#include "../include/comps.h"

namespace rt::comps {
    Comps prepare(const Intersection &intersection, const Ray &ray) {
        Comps comps;
        comps.t = intersection.t;
        comps.object = intersection.object;
        comps.point = ray.at(comps.t);
        comps.eye_vec = -ray.direction;
        comps.normal_vec = comps.object->normal_at(comps.point);

        if (comps.normal_vec.dot(comps.eye_vec) < 0) {
            comps.inside = true;
            comps.normal_vec = -comps.normal_vec;
        } else {
            comps.inside = false;
        }
        comps.over_point = comps.point + comps.normal_vec * math::epsilon;
        comps.reflect_vec = ray.direction.reflect(comps.normal_vec);
        return comps;
    }
}