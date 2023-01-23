//
// Created by Melvic Ybanez on 1/23/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/bounds.h"
#include "../../engine/include/shapes.h"
#include "../include/test_utils.h"
#include "../../engine/math/include/transform.h"

namespace rt::tests::bounds {
    static void init();

    static void addition();

    static void of_shapes();

    static void contains();

    static void transformations();

    void test() {
        set("Bounds", [] {
            init();
            addition();
            of_shapes();
            contains();
            transformations();
        });
    }

    void init() {
        set("Create an empty bounding box", [] {
            Bounds box;
            ASSERT_EQ_MSG("Inspect min", Point(math::infinity, math::infinity, math::infinity), box.min);
            ASSERT_EQ_MSG("Inspect max", Point(-math::infinity, -math::infinity, -math::infinity), box.max);
        });
        set("Create a bounding box with volume", [] {
            Bounds box{Point{-1, -2, -3}, Point{3, 2, 1}};
            ASSERT_EQ_MSG("Inspect min", Point(-1, -2, -3), box.min);
            ASSERT_EQ_MSG("Inspect max", Point(3, 2, 1), box.max);
        });
    }

    void addition() {
        set("Addition", [] {
            set("Adding points to an empty box", [] {
                Bounds box;
                Point p1{-5, 2, 0};
                Point p2{7, 0, -3};
                box = box + p1 + p2;
                ASSERT_EQ_MSG("Inspect min", Point(-5, 0, -3), box.min);
                ASSERT_EQ_MSG("Inspect max", Point(7, 2, 0), box.max);
            });
            scenario("Adding one bounding box to another", [] {
                Bounds box1{Point{-5, -2, 0}, Point{7, 4, 4}};
                Bounds box2{Point{8, -7, -2}, Point{14, 2, 8}};
                box1 = box1 + box2;
                ASSERT_EQ(Bounds(Point(-5, -7, -2), Point(14, 4, 8)), box1);
            });
        });
    }

    void of_shapes() {
        set("Bounding boxes of different shapes", [] {
            scenario("Sphere", [] {
                shapes::Sphere sphere;
                auto box = sphere.bounds();
                ASSERT_EQ(Bounds(Point(-1, -1, -1), Point(1, 1, 1)), box);
            });
            scenario("Plane", [] {
                shapes::Plane plane;
                ASSERT_EQ(Bounds(Point(-math::infinity, 0, -math::infinity), Point(math::infinity, 0, math::infinity)),
                          plane.bounds());
            });
            scenario("Cube", [] {
                shapes::Cube cube;
                auto box = cube.bounds();
                ASSERT_EQ(Bounds(Point(-1, -1, -1), Point(1, 1, 1)), box);
            });
            scenario("Unbounded Cylinder", [] {
                shapes::Cylinder cyl;
                auto box = cyl.bounds();
                ASSERT_EQ(Bounds(Point(-1, -math::infinity, -1), Point(1, math::infinity, 1)), box);
            });
            scenario("Bounded Cylinder", [] {
                shapes::Cylinder cyl{-5, 3};
                auto box = cyl.bounds();
                ASSERT_EQ(Bounds(Point(-1, -5, -1), Point(1, 3, 1)), box);
            });
            scenario("Unbounded Cone", [] {
                shapes::Cone cone;
                auto box = cone.bounds();
                ASSERT_EQ(Bounds(Point(-math::infinity, -math::infinity, -math::infinity),
                                 Point(math::infinity, math::infinity, math::infinity)), box);
            });
            scenario("Bounded Cone", [] {
                shapes::Cone cone{-5, 3};
                auto box = cone.bounds();
                ASSERT_EQ(Bounds(Point(-5, -5, -5), Point(5, 3, 5)), box);
            });
            scenario("Test shape", [] {
                TestShape shape;
                auto box = shape.bounds();
                ASSERT_EQ(Bounds(Point(-1, -1, -1), Point(1, 1, 1)), box);
            });
            scenario("Group", [] {
                auto sphere = new shapes::Sphere;
                math::translate(math::scale(*sphere, 2, 2, 2), 2, 5, -3);
                auto cylinder = new shapes::Cylinder{-2, 2};
                math::translate(math::scale(*cylinder, 0.5, 1, 0.5), -4, -1, 4);
                shapes::Group group;
                group.add_children({sphere, cylinder});
                auto box = group.bounds();
                ASSERT_EQ(Bounds(Point(-4.5, -3, -5), Point(4, 7, 4.5)), box);
            });
        });
        scenario("Querying a shape's bounding box in it's parent's space", [] {
            shapes::Sphere sphere;
            math::translate(math::scale(sphere, 0.5, 2, 4), 1, -3, 5);
            auto box = sphere.parent_space_bounds();
            ASSERT_EQ(Bounds(Point(0.5, -5, 1), Point(1.5, -1, 9)), box);
        });
    }

    void contains() {
        set("Check if the box contains a given point", [] {
            Bounds box{Point{5, -2, 0}, Point{11, 4, 7}};
            std::array<Point, 9> points{
                    Point{5, -2, 0}, Point{11, 4, 7}, Point{8, 1, 3}, Point{3, 0, 3}, Point{8, -4, 3},
                    Point{8, 1, -1}, Point{13, 1, 3}, Point{8, 5, 3}, Point{8, 1, 8}
            };
            std::array<bool, 9> truth_values{true, true, true, false, false, false, false, false, false};

            for (int i = 0; i < points.size(); i++) {
                ASSERT_EQ_MSG(std::to_string(i) + " - Contains point", truth_values[i], box.contains(points[i]));
            }
        });
        set("Check if the box contains another box", [] {
            Bounds box1{Point{5, -2, 0}, Point{11, 4, 7}};
            std::array<Point, 4> mins{
                    Point{5, -2, 0}, Point{6, -1, 1}, Point{4, -3, -1}, Point{6, -1, 1}
            };
            std::array<Point, 4> maxes{
                    Point{11, 4, 7}, Point{10, 3, 6}, Point{10, 3, 6}, Point{12, 5, 6}
            };
            std::array<bool, 4> truth_values{true, true, false, false};

            for (int i = 0; i < mins.size(); i++) {
                Bounds box2{mins[i], maxes[i]};
                ASSERT_EQ_MSG(std::to_string(i) + " - Contains box", truth_values[i], box1.contains(box2));
            }
        });
    }

    void transformations() {
        scenario("Transforming a bounded box", [] {
            Bounds box{Point{-1, -1, -1}, Point{1, 1, 1}};
            box = box.transform(math::matrix::rotation_y(math::pi / 4)).transform(matrix::rotation_x(math::pi / 4));
            ASSERT_EQ(Bounds(Point(-1.4142, -1.7071, -1.7071), Point(1.4142, 1.7071, 1.7071)), box);
        });
    }
}