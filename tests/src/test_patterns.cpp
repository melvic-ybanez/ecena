//
// Created by Melvic Ybanez on 1/5/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/patterns.h"

namespace rt::tests {
    static void stripe();

    void patterns() {
        set("Patterns", [] {
            stripe();
        });
    }

    void stripe() {
        set("Stripe", [] {
            patterns::Stripe pattern{Color::white_, Color::black_};

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
    }
}