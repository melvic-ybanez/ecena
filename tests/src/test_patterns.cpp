//
// Created by Melvic Ybanez on 1/5/23.
//

#include "../../engine/include/shapes.h"
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/patterns.h"
#include "../../engine/math/include/transform.h"

namespace rt::tests {
    static void stripe();

    void patterns() {
        set("Patterns", [] {
            stripe();
        });
    }

    void stripe() {
        patterns::Stripe pattern{Color::white_, Color::black_};

        set("Stripe", [&] {
            scenario("Creating a stripe pattern", [&] {
                ASSERT_EQ(Color::white_, pattern.first);
                ASSERT_EQ(Color::black_, pattern.second);
            });
            scenario("A stripe pattern is constant in y", [&] {
                ASSERT_EQ(Color::white_, pattern.at({0, 0, 0}));
                ASSERT_EQ(Color::white_, pattern.at({0, 1, 0}));
                ASSERT_EQ(Color::white_, pattern.at({0, 2, 0}));
            });
            scenario("A stripe pattern is constant in z", [&] {
                ASSERT_EQ(Color::white_, pattern.at({0, 0, 0}));
                ASSERT_EQ(Color::white_, pattern.at({0, 0, 1}));
                ASSERT_EQ(Color::white_, pattern.at({0, 0, 2}));
            });
            scenario("A stripe pattern alternates i x", [&] {
                ASSERT_EQ(Color::white_, pattern.at({0, 0, 0}));
                ASSERT_EQ(Color::white_, pattern.at({0.9, 0, 0}));
                ASSERT_EQ(Color::black_, pattern.at({1, 0, 0}));
                ASSERT_EQ(Color::black_, pattern.at({-0.1, 0, 0}));
                ASSERT_EQ(Color::black_, pattern.at({-1, 0, 0}));
                ASSERT_EQ(Color::white_, pattern.at({-1.1, 0, 0}));
            });
        });

        set("Transformations", [&] {
            shapes::Sphere sphere;

            scenario("Stripes with an object transformation", [&] {
                math::scale(sphere, 2, 2, 2);
                auto c = pattern.at(sphere, {1.5, 0, 0});
                ASSERT_EQ(Color::white_, c);
            });
            scenario("Stripes with a pattern transformation", [&] {
                patterns::Stripe pattern{Color::white_, Color::black_};
                math::scale(pattern, 2, 2, 2);
                ASSERT_EQ(Color::white_, pattern.at(sphere, {2.5, 0, 0}));
            });
            scenario("Stripes with both an object and a pattern transformation", [&] {
                math::scale(sphere, 2, 2, 2);
                patterns::Stripe pattern{Color::white_, Color::black_};
                math::translate(pattern, 0.5, 0, 0);
                ASSERT_EQ(Color::white_, pattern.at(sphere, {2.5, 0, 0}));
            });
        });
    }
}