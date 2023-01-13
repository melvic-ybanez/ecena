//
// Created by Melvic Ybanez on 1/13/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/shapes.h"

namespace rt::tests::cubes {
    static void intersections();

    static void normals();

    void test() {
        set("Cubes", [] {
            intersections();
            normals();
        });
    }

    void intersections() {
        set("Intersections", [] {
            set("A ray intersects a cube", [] {
                shapes::Cube cube;
                const auto count = 7;

                std::array<Point, count> origins{
                        Point{5, 0.5, 0},   // +x
                        Point{-5, 0.5, 0},  // -x
                        Point{0.5, 5, 0},   // +y
                        Point{0.5, -5, 0},  // -y
                        Point{0.5, 0, 5},   // +z
                        Point{0.5, 0, -5},  // -z
                        Point{0, 0.5, 0}    // inside
                };
                std::array<Vec, count> directions{
                        Vec{-1, 0, 0},  // +x
                        Vec{1, 0, 0},   // -x
                        Vec{0, -1, 0},  // +y
                        Vec{0, 1, 0},   // -y
                        Vec{0, 0, -1},  // +z
                        Vec{0, 0, 1},   // -z
                        Vec{0, 0, 1}  // inside
                };
                std::array<real, count> t1{4, 4, 4, 4, 4, 4, -1};
                std::array<real, count> t2{6, 6, 6, 6, 6, 6, 1};
                std::array<std::string, count> labels = {"+x", "-x", "+y", "-y", "+z", "-z", "inside"};

                for (auto i = 0; i < count; i++) {
                    Ray ray{origins[i], directions[i]};
                    auto xs = cube.local_intersect(ray);

                    auto label = labels[i];
                    ASSERT_EQ_MSG(label + " Count", 2, xs.count());
                    ASSERT_EQ_MSG(label + " T1", t1[i], xs[0]->t);
                    ASSERT_EQ_MSG(label + " T2", t2[i], xs[1]->t);
                }
            });
            set("A ray misses a cube", [] {
                shapes::Cube cube;
                std::array<Point, 6> origins{
                        Point{-2, 0, 0},
                        Point{0, -2, 0},
                        Point{0, 0, -2},
                        Point{2, 0, 2},
                        Point{0, 2, 2},
                        Point{2, 2, 0}
                };
                std::array<Point, origins.size()> directions{
                        Vec{0.2673, 0.5345, 0.8018},
                        Vec{0.8018, 0.2673, 0.5345},
                        Vec{0.5345, 0.8018, 0.2673},
                        Vec{0, 0, -1},
                        Vec{0, -1, 0},
                        Vec{-1, 0, 0}
                };

                for (auto i = 0; i < origins.size(); i++) {
                    Ray ray{origins[i], directions[i]};
                    auto xs = cube.local_intersect(ray);
                    ASSERT_TRUE_MSG(std::to_string(i) + " - Count", xs.empty());
                }
            });
        });
    }

    void normals() {
        set("Normals", [] {
            set("The normal on the surface of a cube", [] {
                shapes::Cube cube;
                std::array<Point, 8> points{
                        Point{1, 0.5, -0.8},
                        Point{-1, -0.2, 0.9},
                        Point{-0.4, 1, -0.1},
                        Point{0.3, -1, -0.7},
                        Point{-0.6, 0.3, 1},
                        Point{0.4, 0.4, -1},
                        Point{1, 1, 1},
                        Point{-1, -1, -1}
                };
                std::array<Vec, points.size()> normals{
                        Vec{1, 0, 0},
                        Vec{-1, 0, 0},
                        Vec{0, 1, 0},
                        Vec{0, -1, 0},
                        Vec{0, 0, 1},
                        Vec{0, 0, -1},
                        Vec{1, 0, 0},
                        Vec{-1, 0, 0}
                };

                for (auto i = 0; i < points.size(); i++) {
                    auto normal = cube.local_normal_at(points[i]);
                    ASSERT_EQ_MSG(std::to_string(i) + " - Normal", normals[i], normal);
                }
            });
        });
    }
}