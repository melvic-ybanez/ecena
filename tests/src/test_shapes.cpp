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

    void test() {
        set("Shapes", [] {
            transformations();
            materials();
            intersections();
            normals();
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
                auto old_mat = std::move(shape.material);
                auto mat = std::make_unique<Material>();
                auto mat1 = std::make_unique<Material>();
                mat->ambient = 1;
                mat1->ambient = 1;
                shape.material = std::move(mat);
                ASSERT_EQ(*mat1, *shape.material);
                ASSERT_NEQ(*shape.material, *old_mat);
            });
        });
    }

    void intersections() {
        set("Intersection", [] {
            scenario("A scaled shape with a ray", [] {
                Ray ray{{0, 0, -5}, {0, 0, 1}};
                TestShape shape;
                math::scale(shape, 2, 2, 2);
                auto xs = shape.intersect(ray);
                Ray expected{{0, 0, -2.5}, {0, 0, 0.5}};

                ASSERT_EQ(expected, shape.saved_ray);
            });
            scenario("A translated shape with a ray", [] {
                Ray ray{{0, 0, -5}, {0, 0, 1}};
                TestShape shape;
                math::translate(shape, 5, 0, 0);
                auto xs = shape.intersect(ray);
                Ray expected{{-5, 0, -5}, {0, 0, 1}};

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
}