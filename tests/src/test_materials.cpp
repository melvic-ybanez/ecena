//
// Created by Melvic Ybanez on 12/23/22.
//

#include <cmath>
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/materials.h"
#include "../../engine/include/light.h"

namespace rt::tests::materials {
    static void default_values();

    static void lighting();

    static void patterns();

    void test() {
        set("Materials", [] {
            default_values();
            lighting();
            patterns();
        });
    }

    void default_values() {
        set("Default", [] {
            Material mat;
            ASSERT_EQ_MSG("Color", Color::white_, mat.color);
            ASSERT_EQ_MSG("Ambient", 0.1, mat.ambient);
            ASSERT_EQ_MSG("Diffuse", 0.9, mat.diffuse);
            ASSERT_EQ_MSG("Specular", 0.9, mat.specular);
            ASSERT_EQ_MSG("Shininess", 200, mat.shininess);
            ASSERT_EQ_MSG("Reflective", 0, mat.reflectivity);
            ASSERT_EQ_MSG("Transparency", 0, mat.transparency);
            ASSERT_EQ_MSG("Refractive Index", 1, mat.refractive_index);
        });
    }

    void lighting() {
        set("Lighting", [] {
            Material mat;
            Point position{0, 0, 0};
            auto light_intensity = Color::white_;
            shapes::Sphere sphere;

            scenario("Lighting with the eye between the light and the surface", [&] {
                Vec eye_vec{0, 0, -1};
                Vec normal_vec{0, 0, -1};
                PointLight light{Point{0, 0, -10}, light_intensity};
                auto result = lights::lighting(sphere, mat, light, position, eye_vec, normal_vec);

                // ambient, diffuse, and specular should be at their peak
                ASSERT_EQ(Color(1.9, 1.9, 1.9), result);
            });
            scenario("Lighting with the eye between light and surface, eye offset 45 degrees", [&] {
                Vec eye_vec{0, std::sqrt(2) / 2, -std::sqrt(2) / 2};
                Vec normal_vec{0, 0, -1};
                PointLight light{Point{0, 0, -10}, light_intensity};
                auto result = lights::lighting(sphere, mat, light, position, eye_vec, normal_vec);

                // specular value should fall to 0
                ASSERT_EQ(Color(1.0, 1.0, 1.0), result);
            });
            scenario("Lighting with eye opposite surface, light offset 45 degrees", [&] {
                Vec eye_vec{0, 0, -1};
                Vec normal_vec{0, 0, -1};
                PointLight light{Point{0, 10, -10}, light_intensity};
                auto result = lights::lighting(sphere, mat, light, position, eye_vec, normal_vec);
                ASSERT_EQ(Color(0.7364, 0.7364, 0.7364), result);
            });
            scenario("Lighting with eye in the path of the reflection vector", [&] {
                Vec eye_vec{0, -std::sqrt(2) / 2, -std::sqrt(2) / 2};
                Vec normal_vec{0, 0, -1};
                PointLight light{Point{0, 10, -10}, light_intensity};
                auto result = lights::lighting(sphere, mat, light, position, eye_vec, normal_vec);

                // like the previous test, but specular is peak
                ASSERT_EQ(Color(1.6364, 1.6364, 1.6364), result);
            });
            scenario("Lighting with the light behind the surface", [&] {
                Vec eye_vec{0, 0, -1};
                Vec normal_vec{0, 0, -1};
                PointLight light{Point{0, 0, 10}, light_intensity};
                auto result = lights::lighting(sphere, mat, light, position, eye_vec, normal_vec);

                // diffuse and specular fall down to 0
                ASSERT_EQ(Color(0.1, 0.1, 0.1), result);
            });
            scenario("Lighting with the surface in shadow", [&] {
                Vec eye_vec{0, 0, -1};
                Vec normal_vec{0, 0, -1};
                PointLight light{{0, 0, -10}, Color::white_};
                auto in_shadow = true;
                auto result = lights::lighting(sphere, mat, light, position, eye_vec, normal_vec, in_shadow);
                ASSERT_EQ(Color(0.1, 0.1, 0.1), result);
            });
        });
    }

    void patterns() {
        set("Patterns", [] {
            set("Lighting with a pattern applied", [] {
                Material mat;
                mat.pattern = std::make_unique<patterns::Stripe>(Color::white_, Color::black_);
                mat.ambient = 1;
                mat.diffuse = 0;
                mat.specular = 0;
                Vec eye_vec{0, 0, -1};
                Vec normal_vec{0, 0, -1};
                PointLight light{{0, 0, -10}, Color::white_};
                shapes::Sphere sphere;

                auto c1 = lights::lighting(sphere, mat, light, {0.9, 0, 0}, eye_vec, normal_vec, false);
                auto c2 = lights::lighting(sphere, mat, light, {1.1, 0, 0}, eye_vec, normal_vec, false);

                ASSERT_EQ_MSG("First color", Color::white_, c1);
                ASSERT_EQ_MSG("Second color", Color::black_, c2);
            });
        });
    }
}