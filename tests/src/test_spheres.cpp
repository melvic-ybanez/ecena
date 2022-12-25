//
// Created by Melvic Ybanez on 12/18/22.
//

#include <cmath>
#include "../include/test_spheres.h"
#include "../include/asserts.h"
#include "../../core/include/ray.h"
#include "../../core/include/shapes.h"

namespace tests::spheres {
    void all() {
        set("Spheres", [] {
            intersections();
            transformations();
            normals();
            materials();
        });
    }

    void intersections() {
        rt::shapes::Sphere sphere;

        set("Intersections", [&] {
            set("A ray intersects the sphere at a tangent", [&] {
                rt::Ray ray{rt::Point{0, 1, -5}, rt::Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                // The intersection only happens at one point, but we still
                // need to store it twice. According to the "Ray Tracer Challenge" book,
                // it will help in determining object overlaps when constructing solid Geometries.
                ASSERT_EQ_MSG("Number of intersections", 2, xs.count());
                ASSERT_EQ_MSG("First intersection", 5.0, xs[0]->t());
                ASSERT_EQ_MSG("Second intersection", 5.0, xs[1]->t());
            });
            scenario("A ray misses the sphere", [&] {
                rt::Ray ray{rt::Point{0, 2, -5}, rt::Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                ASSERT_TRUE_MSG("Number of intersections", xs.empty());
            });
            set("A ray originates inside a sphere", [&] {
                rt::Ray ray{rt::Point{0, 0, 0}, rt::Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                ASSERT_EQ_MSG("Number of intersections", 2, xs.count());
                ASSERT_EQ_MSG("First intersection", -1.0, xs[0]->t());
                ASSERT_EQ_MSG("Second intersection", 1.0, xs[1]->t());
            });
            set("A sphere is behind a ray", [&] {
                rt::Ray ray{rt::Point{0, 0, 5}, rt::Vec{0, 0, 1}};
                auto xs{sphere.intersect(ray)};

                ASSERT_EQ_MSG("Number of intersections", 2, xs.count());
                ASSERT_EQ_MSG("First intersection", -6.0, xs[0]->t());
                ASSERT_EQ_MSG("Second intersection", -4.0, xs[1]->t());
            });
            set("A scaled sphere with a ray", [&]() mutable {
                rt::Ray ray{rt::Point{0, 0, -5}, rt::Vec{0, 0, 1}};
                sphere.transformation = rt::math::matrix::scaling(2, 2, 2);
                auto xs = sphere.intersect(ray);

                ASSERT_EQ_MSG("Inspect count", 2, xs.count());
                ASSERT_EQ_MSG("Inspect first t", 3, xs[0]->t());
                ASSERT_EQ_MSG("Inspect second t", 7, xs[1]->t());
            });
            set("A translated sphere with a ray", [&]() mutable {
                rt::Ray ray{rt::Point{0, 0, -5}, rt::Vec{0, 0, 1}};
                sphere.transformation = rt::math::matrix::translation(5, 0, 0);
                auto xs = sphere.intersect(ray);

                ASSERT_EQ_MSG("Inspect count", 0, xs.count());
            });
        });
    }

    void transformations() {
        rt::shapes::Sphere sphere;

        set("Transformations", [&] {
            auto id = rt::math::matrix::identity<4, 4>();

            ASSERT_EQ_MSG("Default", id, sphere.transformation);

            scenario("Setting a different transformation", [&]() mutable {
                auto t = rt::math::matrix::translation(2, 3, 4);
                sphere.transformation = t;
                ASSERT_EQ(t, sphere.transformation);
            });
        });
    }

    void normals() {
        rt::shapes::Sphere sphere;

        set("Normals", [&] {
            scenario("The normal on a sphere at a point on the x-axis", [&] {
                auto n = sphere.normal_at(rt::Point{1, 0, 0});
                ASSERT_EQ(rt::Vec(1, 0, 0), n);
            });
            scenario("The normal on a sphere at a point on the y-axis", [&] {
                auto n = sphere.normal_at(rt::Point{0, 1, 0});
                ASSERT_EQ(rt::Vec(0, 1, 0), n);
            });
            scenario("The normal on a sphere at a point on the z-axis", [&] {
                auto n = sphere.normal_at(rt::Point{0, 0, 1});
                ASSERT_EQ(rt::Vec(0, 0, 1), n);
            });
            scenario("The normal on a sphere at a nonaxial point", [&] {
                auto v = std::sqrt(3) / 3;
                auto n = sphere.normal_at(rt::Point{v, v, v});
                ASSERT_EQ(rt::Vec(v, v, v), n);
            });
            scenario("The normal is a normalized vector", [&] {
                auto v = std::sqrt(3) / 3;
                auto n = sphere.normal_at(rt::Point{v, v, v});
                ASSERT_EQ(n.normalize(), n);
            });
            scenario("Computing the normal on a translated sphere", [] {
                rt::shapes::Sphere sphere;
                sphere.translate(0, 1, 0);
                auto n = sphere.normal_at(rt::Point{0, 1.70711, -0.70711});
                ASSERT_EQ(rt::Vec(0, 0.70711, -0.70711), n);
            });
            scenario("Computing the normal on a transformed sphere", [] {
                rt::shapes::Sphere sphere;
                sphere.rotate_z(rt::math::pi / 5).scale(1, 0.5, 1);
                auto n = sphere.normal_at(rt::Point{0, std::sqrt(2) / 2, -std::sqrt(2) / 2});
                ASSERT_EQ(rt::Vec(0, 0.97014, -0.24254), n);
            });
        });
    }

    void materials() {
        set("Materials", [] {
            scenario("A sphere has a default material", [] {
                rt::shapes::Sphere sphere;
                ASSERT_EQ(sphere.material, rt::Material{});
            });
            set("A sphere may be assigned material", [] {
                rt::shapes::Sphere sphere;
                rt::Material mat;
                mat.ambient = 1;

                ASSERT_TRUE_MSG("Material is not equal to default", sphere.material != mat);

                sphere.material = mat;
                ASSERT_EQ_MSG("Material is equal to the sphere's material ", mat, sphere.material);
            });
        });
    }
}