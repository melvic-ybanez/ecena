//
// Created by Melvic Ybanez on 1/21/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/shapes.h"
#include "../include/test_utils.h"
#include "../../engine/math/include/transform.h"

namespace rt::tests::groups {
    static void init();

    static void children();

    static void intersections();

    static void normals();

    void test() {
        set("Groups", [] {
            init();
            children();
            intersections();
            normals();
        });
    }

    void init() {
        set("Creating a new group", [] {
            shapes::Group group;
            auto expected_transform = matrix::identity<4, 4>();

            ASSERT_EQ_MSG("Default transform", expected_transform, group.transformation);
            ASSERT_TRUE_MSG("Group is empty", group.empty());
        });
        scenario("A shape has a prent", [] {
            TestShape shape;
            ASSERT_EQ(nullptr, shape.parent);
        });
    }

    void children() {
        set("Adding a child to a group", [] {
            shapes::Group group;
            auto child = new TestShape;
            group.add_child(child);

            ASSERT_FALSE_MSG("Group is not empty", group.empty());
            ASSERT_TRUE_MSG("Group contains the added child", group.contains(child));
            ASSERT_EQ_MSG("The group is the added child's parent", &group, child->parent);
        });
    }

    void intersections() {
        set("Intersections", [] {
            scenario("With an empty group", [] {
                shapes::Group group;
                Ray ray{Point{0, 0, 0}, Vec{0, 0, 1}};
                auto xs = group.local_intersect(ray);
                ASSERT_TRUE(xs.empty());
            });
            set("With a non-empty group", [] {
                shapes::Group group;
                auto s1 = new shapes::Sphere;
                auto s2 = new shapes::Sphere;
                auto s3 = new shapes::Sphere;

                math::translate(*s2, 0, 0, -3);
                math::translate(*s3, 5, 0, 0);

                group.add_children({s1, s2, s3});

                Ray ray{Point{0, 0, -5}, Vec{0, 0, 1}};
                auto xs = group.local_intersect(ray);

                ASSERT_EQ_MSG("Count", 4, xs.count());
                ASSERT_EQ_MSG("1st object of intersection", xs[0]->object, s2);
                ASSERT_EQ_MSG("2nd object of intersection", xs[1]->object, s2);
                ASSERT_EQ_MSG("erd object of intersection", xs[2]->object, s1);
                ASSERT_EQ_MSG("4th object of intersection", xs[3]->object, s1);
            });
            set("with a transformed group", [] {
                shapes::Group group;
                auto sphere = new shapes::Sphere;

                math::translate(*sphere, 5, 0, 0);
                math::scale(group, 2, 2, 2);

                group.add_child(sphere);
                Ray ray{Point{10, 0, -10}, Vec{0, 0, 1}};
                auto xs = group.intersect(ray);

                ASSERT_EQ(2, xs.count());
            });
        });
    }

    void normals() {
        set("Normals", [] {
            scenario("Finding the normal on a child object", [] {
                shapes::Group g1;
                auto g2 = new shapes::Group;
                auto sphere = new shapes::Sphere;

                math::rotate_y(g1, math::pi / 2);
                math::scale(*g2, 1, 2, 3);
                math::translate(*sphere, 5, 0, 0);

                g1.add_child(g2);
                g2->add_child(sphere);

                auto n = sphere->normal_at(Point{1.7321, 1.1547, -5.5774});
                ASSERT_EQ(Vec(0.2857, 0.4286, -0.8571), n);
            });
        });
    }
}