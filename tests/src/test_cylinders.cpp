//
// Created by Melvic Ybanez on 1/14/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/shapes.h"

namespace rt::tests::cylinders {
    static void init();

    static void intersections();

    static void normals();

    void test() {
        set("Cylinders", [] {
            init();
            intersections();
            normals();
        });
    }

    void init() {
        set("The default minimum and maximum for a cylinder", [] {
            shapes::Cylinder cylinder;
            ASSERT_EQ_MSG("Minimum bound", -math::infinity, cylinder.minimum);
            ASSERT_EQ_MSG("Maximum bound", math::infinity, cylinder.maximum);
            ASSERT_FALSE_MSG("Closed", cylinder.closed);
        });
    }

    void intersections() {
        set("Intersections", [] {
            set("A ray misses a cylinder", [] {
                shapes::Cylinder cylinder;
                std::array<Point, 3> origins{
                        Point{1, 0, 0}, Point{0, 0, 0}, Point{0, 0, -5}
                };
                std::array<Vec, 3> directions{
                        Vec{0, 1, 0}, Vec{0, 1, 0}, Vec{1, 1, 1}
                };

                for (int i = 0; i < origins.size(); i++) {
                    auto direction = directions[i].normalize();
                    Ray ray{origins[i], direction};
                    auto xs = cylinder.local_intersect(ray);
                    ASSERT_TRUE_MSG(std::to_string(i) + " Count", xs.empty());
                }
            });
            set("A ray strikes a cylinder", [] {
                shapes::Cylinder cylinder;
                const auto size = 3;
                std::array<Point, size> origins{
                        Point{1, 0, -5}, Point{0, 0, -5}, Point{0.5, 0, -5}
                };
                std::array<Vec, size> directions{
                        Vec{0, 0, 1}, Vec{0, 0, 1}, Vec{0.1, 1, 1}
                };
                std::array<real, size> t0s{5, 4, 6.80798};
                std::array<real, size> t1s{5, 6, 7.08872};

                for (auto i = 0; i < size; i++) {
                    auto direction = directions[i].normalize();
                    Ray ray{origins[i], direction};
                    auto xs = cylinder.local_intersect(ray);

                    auto label = std::to_string(i);
                    ASSERT_EQ_MSG(label + " - Count", 2, xs.count());
                    ASSERT_TRUE_MSG(label + " - First Intersection", math::compare_reals(t0s[i], xs[0]->t));
                    ASSERT_TRUE_MSG(label + " - Second Intersection", math::compare_reals(t1s[i], xs[1]->t));
                }
            });

            // Captures six scenarios
            // Scenario 1: Ray originates from the inside
            // Scenario 2: Perpendicular to the y-axis, from above cylinder
            // Scenario 3: Like #2, but from below the cylinder
            // Scenario 4: Upper edge of the cylinder
            // Scenario 5: Lower edge of the cylinder
            // Scenario 6: Perpendicular to through the middle
            set("Intersecting a constrained cylinder", [] {
                shapes::Cylinder cylinder{1, 2};

                std::array<Point, 6> points{
                        Point{0, 1.5, 0}, Point{0, 3, -5}, Point{0, 0, -5},
                        Point{0, 2, -5}, Point{0, 1, -5}, Point{0, 1.5, -2}
                };
                std::array<Vec, 6> directions{
                        Vec{0.1, 1, 0}, Vec{0, 0, 1}, Vec{0, 0, 1},
                        Vec{0, 0, 1}, Vec{0, 0, 1}, Vec{0, 0, 1}
                };
                std::array<real, 6> counts{
                        0, 0, 0, 0, 0, 2
                };

                for (int i = 0; i < points.size(); i++) {
                    auto direction = directions[i].normalize();
                    Ray ray{points[i], direction};
                    auto xs = cylinder.local_intersect(ray);
                    ASSERT_EQ_MSG(std::to_string(i) + " - Count", counts[i], xs.count());
                }
            });

            set("Intersecting the caps of a closed cylinder", [] {
                shapes::Cylinder cylinder{1, 2, true};
                std::array<Point, 5> origins{
                        Point{0, 3, 0}, Point{0, 3, -2}, Point{0, 4, -2},
                        Point{0, 0, -2}, Point{0, -1, -2}
                };
                std::array<Vec, 5> directions{
                    Vec{0, -1, 0}, Vec{0, -1, 2}, Vec{0, -1, 1},
                    Vec{0, 1, 2}, Vec{0, 1, 1}
                };

                for (int i = 0; i < origins.size(); i++) {
                    auto direction = directions[i].normalize();
                    Ray ray{origins[i], direction};
                    auto xs = cylinder.local_intersect(ray);

                    ASSERT_EQ_MSG(std::to_string(i) + " - Count", 2, xs.count());
                }
            });
        });
    }

    void normals() {
        set("Normals", [] {
            set("Normal vector on a cylinder", [] {
                shapes::Cylinder cylinder;
                std::array<Point, 4> points{
                        Point{1, 0, 0}, Point{0, 5, -1}, Point{0, -2, 1}, Point{-1, 1, 0}
                };
                std::array<Vec, 4> normals{
                        Vec{1, 0, 0}, Vec{0, 0, -1}, Vec{0, 0, 1}, Vec{-1, 0, 0}
                };

                for (int i = 0; i < points.size(); i++) {
                    auto normal = cylinder.local_normal_at(points[i]);
                    ASSERT_EQ_MSG(std::to_string(i) + " - Normal", normals[i], normal);
                }
            });
            set("The normal vector on a cylinder's end caps", [] {
                shapes::Cylinder cylinder{1, 2, true};
                std::array<Point, 6> points{
                    Point{0, 1, 0}, Point{0.5, 1, 0}, Point{0, 1, 0.5},
                    Point{0, 2, 0}, Point{0.5, 2, 0}, Point{0, 2, 0.5}
                };
                std::array<Vec, 6> normals{
                    Vec{0, -1, 0}, Vec{0, -1, 0}, Vec{0, -1, 0},
                    Vec{0, 1, 0}, Vec{0, 1, 0}, Vec{0, 1, 0}
                };

                for (int i = 0; i < points.size(); i++) {
                    auto normal = cylinder.local_normal_at(points[i]);
                    ASSERT_EQ_MSG(std::to_string(i) + " - Normal", normals[i], normal);
                }
            });
        });
    }
}