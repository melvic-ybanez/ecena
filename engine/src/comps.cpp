//
// Created by Melvic Ybanez on 12/31/22.
//

#include <cmath>
#include "../include/comps.h"

namespace rt::comps {
    static void compute_n1_and_n2(Comps& comps, const Intersection& hit, Aggregate& aggregate);

    Comps prepare(const Intersection& hit, const Ray& ray, Aggregate& aggregate) {
        Comps comps;
        comps.t = hit.t;
        comps.object = hit.object;
        comps.point = ray.at(comps.t);
        comps.eye_vec = -ray.direction;
        comps.normal_vec = comps.object->normal_at(comps.point);

        if (comps.normal_vec.dot(comps.eye_vec) < 0) {
            comps.inside = true;
            comps.normal_vec = -comps.normal_vec;
        } else {
            comps.inside = false;
        }

        auto normal_x_epsilon = comps.normal_vec * math::epsilon;
        comps.over_point = comps.point + normal_x_epsilon;
        comps.under_point = comps.point - normal_x_epsilon;

        comps.reflect_vec = ray.direction.reflect(comps.normal_vec);

        compute_n1_and_n2(comps, hit, aggregate);

        return comps;
    }

    Comps prepare(const Intersection& hit, const Ray& ray) {
        Aggregate agg{{new Intersection(hit.t, hit.object)}};
        return prepare(hit, ray, agg);
    }

    void compute_n1_and_n2(Comps& comps, const Intersection& hit, Aggregate& aggregate) {
        std::vector<Shape*> containers;
        for (auto i: aggregate.elems) {
            if (i == &hit) {
                if (containers.empty()) comps.n1 = 1.0;
                else comps.n1 = containers.back()->material->refractive_index;
            }

            if (std::find(containers.begin(), containers.end(), i->object) != containers.end()) {
                // the intersection is exiting the object
                containers.erase(std::remove(containers.begin(), containers.end(), i->object), containers.end());
            } else {
                // the intersection is entering the object
                containers.push_back(i->object);
            }

            if (i == &hit) {
                if (containers.empty()) comps.n2 = 1.0;
                else comps.n2 = containers.back()->material->refractive_index;

                break;
            }
        }
    }

    real schlick(const Comps& comps) {
        auto cos = comps.eye_vec.dot(comps.normal_vec);

        // total internal reflection
        if (comps.n1 > comps.n2) {
            auto n = comps.n1 / comps.n2;
            auto sin2_t = n * n * (1 - cos * cos);
            if (sin2_t > 1) return 1;

            cos = std::sqrt(1 - sin2_t);
        }

        auto r = std::pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);
        return r + (1 - r) * std::pow(1 - cos, 5);
    }
}