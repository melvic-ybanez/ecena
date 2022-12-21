//
// Created by Melvic Ybanez on 12/18/22.
//

#include "../include/test_spheres.h"
#include "../include/asserts.h"
#include "../../core/include/ray.h"
#include "../../core/include/shapes.h"

namespace tests::spheres {
    void all() {
        set("Spheres", [] {
            intersections();
            transformations();
        });
    }

    void intersections() {
        rt::shapes::Sphere sphere;

        set("Intersections", [=] {
            set("A ray intersects the sphere at a tangent", [=] {
                rt::Ray ray{rt::Point{0, 1, -5}, rt::Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                // The intersection only happens at one point, but we still
                // need to store it twice. According to the "Ray Tracer Challenge" book,
                // it will help in determining object overlaps when constructing solid Geometries.
                ASSERT_EQ_MSG("Number of intersections", 2, xs.count());
                ASSERT_EQ_MSG("First intersection", 5.0, xs[0]->t());
                ASSERT_EQ_MSG("Second intersection", 5.0, xs[1]->t());
            });
            scenario("A ray misses the sphere", [=] {
                rt::Ray ray{rt::Point{0, 2, -5}, rt::Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                ASSERT_TRUE_MSG("Number of intersections", xs.empty());
            });
            set("A ray originates inside a sphere", [=] {
                rt::Ray ray{rt::Point{0, 0, 0}, rt::Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                ASSERT_EQ_MSG("Number of intersections", 2, xs.count());
                ASSERT_EQ_MSG("First intersection", -1.0, xs[0]->t());
                ASSERT_EQ_MSG("Second intersection", 1.0, xs[1]->t());
            });
            set("A sphere is behind a ray", [=] {
                rt::Ray ray{rt::Point{0, 0, 5}, rt::Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                ASSERT_EQ_MSG("Number of intersections", 2, xs.count());
                ASSERT_EQ_MSG("First intersection", -6.0, xs[0]->t());
                ASSERT_EQ_MSG("Second intersection", -4.0, xs[1]->t());
            });
        });
    }

    void transformations() {
        rt::shapes::Sphere sphere;

        set("Transformations", [=] {
            auto id = rt::math::matrix::identity<4, 4>();

            ASSERT_EQ_MSG("Default", id, sphere.transformation);

            scenario("Setting a different transformation", [=] () mutable {
                auto t = rt::math::matrix::translation(2, 3, 4);
                sphere.transformation = t;
                ASSERT_EQ(t, sphere.transformation);
            });
        });
    }
}