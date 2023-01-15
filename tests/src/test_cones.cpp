//
// Created by Melvic Ybanez on 1/15/23.
//

#include <cmath>
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/shapes.h"

namespace rt::tests::cones {
    static void intersections();

    static void normals();

    void test() {
        set("Cones", [] {
            intersections();
            normals();
        });
    }

    void intersections() {
        set("Intersections", [] {
            set("Intersecting a cone with a ray", [] {
                shapes::Cone cone;
                std::array<Point, 3> origins{
                    Point{0, 0, -5}, Point{0, 0, -5}, Point{1, 1, -5}
                };
                std::array<Vec, 3> directions{
                    Vec{0, 0, 1}, Vec{1, 1, 1}, Vec{-0.5, -1, 1}
                };
                std::array<real, 3> t0s{5, 8.66025, 4.55006};
                std::array<real, 3> t1s{5, 8.66025, 49.44994};

                for (int i = 0; i < origins.size(); i++) {
                    auto direction = directions[i].normalize();
                    Ray ray{origins[i], direction};
                    auto xs = cone.local_intersect(ray);

                    auto label = std::to_string(i);
                    ASSERT_EQ_MSG(label + " - Count", 2, xs.count());
                    ASSERT_EQR_MSG(label + " - First intersection", t0s[i], xs[0]->t);
                    ASSERT_EQR_MSG(label + " - Second intersection", t1s[i], xs[1]->t);
                }
            });
            set("Intersecting a cone with a ray parallel to one of its halves", [] {
                shapes::Cone cone;
                auto direction = Vec{0, 1, 1}.normalize();
                Ray ray{Point{0, 0, -1}, direction};
                auto xs = cone.local_intersect(ray);

                ASSERT_EQ_MSG("Count", 1, xs.count());
                ASSERT_EQR_MSG("First intersection", 0.35355, xs[0]->t);
            });
        });
    }

    void normals() {
        set("Normals", [] {
            set("Computing the normal vector on a cone", [] {
                shapes::Cone cone;
                std::array<Point, 3> points{
                    Point{0, 0, 0}, Point{1, 1, 1}, Point{-1, -1, 0}
                };
                std::array<Vec, 3> normals{
                    Vec{0, 0, 0}, Vec{1, -std::sqrt(2), 1}, Vec{-1, 1, 0}
                };

                for (int i = 0; i < points.size(); i++) {
                    auto normal = cone.local_normal_at(points[i]);
                    ASSERT_EQ_MSG(std::to_string(i) + " - Normal", normals[i], normal);
                }
            });
        });
    }
}