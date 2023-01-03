//
// Created by Melvic Ybanez on 12/18/22.
//

#include <cmath>
#include "../include/test_spheres.h"
#include "../include/asserts.h"
#include "../../engine/include/ray.h"
#include "../../engine/include/shapes.h"

namespace rt::tests::spheres {
    void all() {
        set("Spheres", [] {
            intersections();
            normals();
        });
    }

    void intersections() {
        rt::shapes::Sphere sphere;

        set("Intersections", [&] {
            set("A ray intersects the sphere at a tangent", [&] {
                Ray ray{Point{0, 1, -5}, Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                // The intersection only happens at one point, but we still
                // need to store it twice. According to the "Ray Tracer Challenge" book,
                // it will help in determining object overlaps when constructing solid Geometries.
                ASSERT_EQ_MSG("Number of intersections", 2, xs.count());
                ASSERT_EQ_MSG("First intersection", 5.0, xs[0]->t);
                ASSERT_EQ_MSG("Second intersection", 5.0, xs[1]->t);
            });
            scenario("A ray misses the sphere", [&] {
                Ray ray{Point{0, 2, -5}, Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                ASSERT_TRUE_MSG("Number of intersections", xs.empty());
            });
            set("A ray originates inside a sphere", [&] {
                Ray ray{Point{0, 0, 0}, Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                ASSERT_EQ_MSG("Number of intersections", 2, xs.count());
                ASSERT_EQ_MSG("First intersection", -1.0, xs[0]->t);
                ASSERT_EQ_MSG("Second intersection", 1.0, xs[1]->t);
            });
            set("A sphere is behind a ray", [&] {
                Ray ray{Point{0, 0, 5}, Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                ASSERT_EQ_MSG("Number of intersections", 2, xs.count());
                ASSERT_EQ_MSG("First intersection", -6.0, xs[0]->t);
                ASSERT_EQ_MSG("Second intersection", -4.0, xs[1]->t);
            });
            set("A scaled sphere with a ray", [&]() mutable {
                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                sphere.transformation = math::matrix::scaling(2, 2, 2);
                auto xs = sphere.intersect(ray);

                ASSERT_EQ_MSG("Inspect count", 2, xs.count());
                ASSERT_EQ_MSG("Inspect first t", 3, xs[0]->t);
                ASSERT_EQ_MSG("Inspect second t", 7, xs[1]->t);
            });
            set("A translated sphere with a ray", [&]() mutable {
                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                sphere.transformation = math::matrix::translation(5, 0, 0);
                auto xs = sphere.intersect(ray);

                ASSERT_EQ_MSG("Inspect count", 0, xs.count());
            });
        });
    }

    void normals() {
        rt::shapes::Sphere sphere;

        set("Normals", [&] {
            scenario("The normal on a sphere at a point on the x-axis", [&] {
                auto n = sphere.normal_at(Point{1, 0, 0});
                ASSERT_EQ(Vec(1, 0, 0), n);
            });
            scenario("The normal on a sphere at a point on the y-axis", [&] {
                auto n = sphere.normal_at(Point{0, 1, 0});
                ASSERT_EQ(Vec(0, 1, 0), n);
            });
            scenario("The normal on a sphere at a point on the z-axis", [&] {
                auto n = sphere.normal_at(Point{0, 0, 1});
                ASSERT_EQ(Vec(0, 0, 1), n);
            });
            scenario("The normal on a sphere at a nonaxial point", [&] {
                auto v = std::sqrt(3) / 3;
                auto n = sphere.normal_at(Point{v, v, v});
                ASSERT_EQ(Vec(v, v, v), n);
            });
            scenario("The normal is a normalized vector", [&] {
                auto v = std::sqrt(3) / 3;
                auto n = sphere.normal_at(Point{v, v, v});
                ASSERT_EQ(n.normalize(), n);
            });
            scenario("Computing the normal on a translated sphere", [] {
                rt::shapes::Sphere sphere;
                sphere.translate(0, 1, 0);
                auto n = sphere.normal_at(Point{0, 1.70711, -0.70711});
                ASSERT_EQ(Vec(0, 0.70711, -0.70711), n);
            });
            scenario("Computing the normal on a transformed sphere", [] {
                rt::shapes::Sphere sphere;
                sphere.rotate_z(math::pi / 5).scale(1, 0.5, 1);
                auto n = sphere.normal_at(Point{0, std::sqrt(2) / 2, -std::sqrt(2) / 2});
                ASSERT_EQ(Vec(0, 0.97014, -0.24254), n);
            });
        });
    }
}