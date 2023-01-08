//
// Created by Melvic Ybanez on 12/31/22.
//

#include "../include/comps.h"

namespace rt::comps {
    static void compute_n1_and_n2(Comps &comps, const Intersection &hit, Aggregate &aggregate);

    Comps prepare(const Intersection &hit, const Ray &ray, Aggregate &aggregate) {
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

        auto normal_epsilon_prod = comps.normal_vec * math::epsilon;
        comps.over_point = comps.point + normal_epsilon_prod;
        comps.under_point = comps.point - normal_epsilon_prod;

        comps.reflect_vec = ray.direction.reflect(comps.normal_vec);

        compute_n1_and_n2(comps, hit, aggregate);

        return comps;
    }

    Comps prepare(const Intersection &hit, const Ray &ray) {
        Aggregate agg{{new Intersection(hit.t, hit.object)}};
        return prepare(hit, ray, agg);
    }

    void compute_n1_and_n2(Comps &comps, const Intersection &hit, Aggregate &aggregate) {
        std::vector<Shape *> containers;
        for (auto i : aggregate.elems) {
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
}