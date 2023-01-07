//
// Created by Melvic Ybanez on 12/31/22.
//

#include "../include/comps.h"

namespace rt {
    Comps::Comps(const Intersection &intersection, const Ray &ray)
            : t{intersection.t},
              object{intersection.object},
              point{ray.at(t)},
              eye_vec{-ray.direction},
              normal_vec{object->normal_at(point)} {
        if (normal_vec.dot(eye_vec) < 0) {
            inside = true;
            normal_vec = -normal_vec;
        } else {
            inside = false;
        }
        over_point = point + normal_vec * math::epsilon;
        reflect_vec = ray.direction.reflect(normal_vec);
    }
}