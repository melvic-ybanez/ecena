//
// Created by Melvic Ybanez on 2/2/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/math/include/tuples.h"
#include "../../engine/include/shapes.h"

namespace rt::tests::triangles {
    static void init();

    static void normals();

    static void intersections();

    static void  bounds();

    void test() {
        set("Triangles", [] {
            init();
            normals();
            intersections();
            bounds();
        });
    }

    void init() {
        set("Constructing a triangle", [] {
            Point p1{0, 1, 0};
            Point p2{-1, 0, 0};
            Point p3{1, 0, 0};
            shapes::Triangle t{p1, p2, p3};

            ASSERT_EQ_MSG("P1", p1, t.p1);
            ASSERT_EQ_MSG("P2", p2, t.p2);
            ASSERT_EQ_MSG("P3", p3, t.p3);
            ASSERT_EQ_MSG("E1", Vec(-1, -1, 0), t.e1);
            ASSERT_EQ_MSG("E2", Vec(1, -1, 0), t.e2);
            ASSERT_EQ_MSG("Normal", Vec(0, 0, -1), t.normal);
        });
    }

    void normals() {
        set("Finding the normal on a triangle", [] {
            shapes::Triangle t{{0,  1, 0},
                               {-1, 0, 0},
                               {1,  0, 0}};
            auto n1 = t.local_normal_at({0, 0.5, 0});
            auto n2 = t.local_normal_at({-0.5, 0.75, 0});
            auto n3 = t.local_normal_at({0.5, 0.25, 0});

            ASSERT_EQ_MSG("N1", n1, t.normal);
            ASSERT_EQ_MSG("N2", n2, t.normal);
            ASSERT_EQ_MSG("N3", n3, t.normal);
        });
    }

    void intersections() {
        set("Intersections", [] {
            scenario("Intersection a ray parallel to the triangle", [] {
                shapes::Triangle t{Point{0, 1, 0}, Point{-1, 0, 0}, Point{1, 0, 0}};
                Ray ray{Point{0, -1, -2}, Vec{0, 1, 0}};
                auto xs = t.local_intersect(ray);
                ASSERT_TRUE(xs.empty());
            });
            scenario("A ray misses the p1-p3 edge", [] {
                shapes::Triangle t{Point{0, 1, 0}, Point{-1, 0, 0}, Point{1, 0, 0}};
                Ray ray{Point{1, 1, -2}, Vec{0, 0, 1}};
                auto xs = t.local_intersect(ray);
                ASSERT_TRUE(xs.empty());
            });
            scenario("A ray misses the p1-p2 edge", [] {
                shapes::Triangle t{Point{0, 1, 0}, Point{-1, 0, 0}, Point{1, 0, 0}};
                Ray ray{Point{-1, 1, -2}, Vec{0, 0, 1}};
                auto xs = t.local_intersect(ray);
                ASSERT_TRUE(xs.empty());
            });
            scenario("A ray misses the p2-p3 edge", [] {
                shapes::Triangle t{Point{0, 1, 0}, Point{-1, 0, 0}, Point{1, 0, 0}};
                Ray ray{Point{0, -1, -2}, Vec{0, 0, 1}};
                auto xs = t.local_intersect(ray);
                ASSERT_TRUE(xs.empty());
            });
            set("A ray strikes a triangle", [] {
                shapes::Triangle t{Point{0, 1, 0}, Point{-1, 0, 0}, Point{1, 0, 0}};
                Ray ray{Point{0, 0.5, -2}, Vec{0, 0, 1}};
                auto xs = t.local_intersect(ray);
                ASSERT_EQ_MSG("Count", 1, xs.count());
                ASSERT_EQ_MSG("t", 2, xs[0]->t);
            });
        });
    }

    void bounds() {
        scenario("A triangle has a bounding box", [] {
            Point p1{-3, 7, 2};
            Point p2{6, 2, -4};
            Point p3{2, -1, -1};
            shapes::Triangle triangle{p1, p2, p3};
            auto box = triangle.bounds();
            ASSERT_EQ(Bounds(Point(-3, -1, -4), Point(6, 7, 2)), box);
        });
    }
}