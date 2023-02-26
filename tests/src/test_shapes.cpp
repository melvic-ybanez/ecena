//
// Created by Melvic Ybanez on 1/3/23.
//

#include <cmath>
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/shapes.h"
#include "../include/test_utils.h"
#include "../../engine/math/include/transform.h"

namespace rt::tests::shapes_ {
    static void transformations();

    static void materials();

    static void intersections();

    static void normals();

    static void space_conversions();

    void test() {
        set("Shapes", [] {
            transformations();
            materials();
            intersections();
            normals();
            space_conversions();
        });
    }

    void transformations() {
        scenario("Default transformation", [] {
            TestShape shape;
            auto expected = matrix::identity<4, 4>();
            ASSERT_EQ(expected, shape.transformation);
        });
        scenario("Assigning a transformation", [] {
            TestShape shape;
            math::translate(shape, 2, 3, 4);
            ASSERT_EQ(matrix::translation(2, 3, 4), shape.transformation);
        });
    }

    void materials() {
        set("Material", [] {
            scenario("Default", [] {
                TestShape shape;
                Material mat;
                ASSERT_EQ(mat, *shape.material);
            });
            scenario("Update", [] {
                TestShape shape;
                auto old_mat = shape.material;
                auto mat = std::make_unique<Material>();
                auto mat1 = std::make_unique<Material>();
                mat->ambient = 1;
                mat1->ambient = 1;
                shape.material = mat.get();
                ASSERT_EQ(*mat1, *shape.material);
                ASSERT_NEQ(*shape.material, *old_mat);
            });
        });
    }

    void intersections() {
        set("Intersection", [] {
            scenario("A scaled shape with a ray", [] {
                Ray ray{{0, 0, -5},
                        {0, 0, 1}};
                TestShape shape;
                math::scale(shape, 2, 2, 2);
                auto xs = shape.intersect(ray);
                Ray expected{{0, 0, -2.5},
                             {0, 0, 0.5}};

                ASSERT_EQ(expected, shape.saved_ray);
            });
            scenario("A translated shape with a ray", [] {
                Ray ray{{0, 0, -5},
                        {0, 0, 1}};
                TestShape shape;
                math::translate(shape, 5, 0, 0);
                auto xs = shape.intersect(ray);
                Ray expected{{-5, 0, -5},
                             {0,  0, 1}};

                ASSERT_EQ(expected, shape.saved_ray);
            });
        });
    }

    void normals() {
        set("Normals", [] {
            scenario("On a translated shape", [] {
                TestShape shape;
                math::translate(shape, 0, 1, 0);
                auto normal = shape.normal_at({0, 1.70711, -0.70711});
                Vec expected{0, 0.70711, -0.70711};
                ASSERT_EQ(expected, normal);
            });
            scenario("On a transformed shape", [] {
                TestShape shape;
                math::scale(math::rotate_z(shape, math::pi / 5), 1, 0.5, 1);
                auto normal = shape.normal_at({0, std::sqrt(2) / 2, -std::sqrt(2) / 2});
                Vec expected{0, 0.97014, -0.24254};
                ASSERT_EQ(expected, normal);
            });
        });
    }

    void space_conversions() {
        scenario("Converting a point from world to object space", [] {
            shapes::Group g1;
            math::rotate_y(g1, math::pi / 2);

            auto g2 = new shapes::Group;
            math::scale(*g2, 2, 2, 2);

            g1.add_child(g2);

            auto sphere = new shapes::Sphere;
            math::translate(*sphere, 5, 0, 0);

            g2->add_child(sphere);

            auto point = sphere->world_to_object(Point{-2, 0, -10});
            ASSERT_EQ(Point(0, 0, -1), point);
        });
        scenario("Converting a normal from object to world space", [] {
            shapes::Group g1;
            math::rotate_y(g1, math::pi / 2);

            auto g2 = new shapes::Group;
            math::scale(*g2, 1, 2, 3);
            g1.add_child(g2);

            auto sphere = new shapes::Sphere;
            math::translate(*sphere, 5, 0, 0);
            g2->add_child(sphere);

            auto c = std::sqrt(3) / 3;
            auto n = sphere->normal_to_world(Vec{c, c, c});
            ASSERT_EQ(Vec(0.2857, 0.4286, -0.8571), n);
        });
    }
}