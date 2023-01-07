//
// Created by Melvic Ybanez on 1/5/23.
//

#include "../../engine/include/shapes.h"
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/patterns.h"
#include "../../engine/math/include/transform.h"
#include "../include/test_utils.h"

namespace rt::tests::patterns_ {
    static void test_pattern();

    static void stripe();

    static void gradient();

    void test() {
        set("Patterns", [] {
            stripe();
            test_pattern();
            gradient();
        });
    }

    void test_pattern() {
        scenario("The default pattern transformation", [] {
            TestPattern pattern;
            auto expected = matrix::identity<4, 4>();
            ASSERT_EQ(expected, pattern.transformation);
        });
        scenario("Assigning a transformation", [] {
            TestPattern pattern;
            math::translate(pattern, 1, 2, 3);
            ASSERT_EQ(matrix::translation(1, 2, 3), pattern.transformation);
        });
        scenario("A pattern with an object transformation", [] {
            shapes::Sphere sphere;
            TestPattern pattern;
            math::scale(sphere, 2, 2, 2);
            auto color = pattern.at(sphere, {2, 3, 4});
            ASSERT_EQ(Color(1, 1.5, 2), color);
        });
        scenario("A pattern with a pattern transformation", [] {
            shapes::Sphere sphere;
            TestPattern pattern;
            math::scale(pattern, 2, 2, 2);
            auto color = pattern.at(sphere, {2, 3, 4});
            ASSERT_EQ(Color(1, 1.5, 2), color);
        });
        scenario("A pattern with both an object and a pattern transformation", [] {
            shapes::Sphere sphere;
            TestPattern pattern;
            math::scale(sphere, 2, 2, 2);
            math::translate(pattern, 0.5, 1, 1.5);
            auto color = pattern.at(sphere, {2.5, 3, 3.5});
            ASSERT_EQ(Color(0.75, 0.5, 0.25), color);
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
            scenario("Stripes with an object transformation", [&] {
                shapes::Sphere sphere;
                math::scale(sphere, 2, 2, 2);
                auto c = pattern.at(sphere, {1.5, 0, 0});
                ASSERT_EQ(Color::white_, c);
            });
            scenario("Stripes with a pattern transformation", [&] {
                shapes::Sphere sphere;
                patterns::Stripe pattern{Color::white_, Color::black_};
                math::scale(pattern, 2, 2, 2);
                ASSERT_EQ(Color::white_, pattern.at(sphere, {1.5, 0, 0}));
            });
            scenario("Stripes with both an object and a pattern transformation", [&] {
                shapes::Sphere sphere;
                math::scale(sphere, 2, 2, 2);
                patterns::Stripe pattern{Color::white_, Color::black_};
                math::translate(pattern, 0.5, 0, 0);
                ASSERT_EQ(Color::white_, pattern.at(sphere, {2.5, 0, 0}));
            });
        });
    }

    void gradient() {
        set("Gradient", [] {
            set("A gradient linearly interpolates between colors", [] {
                patterns::Gradient pattern{Color::white_, Color::black_};
                ASSERT_EQ_MSG("{0, 0, 0}", Color::white_, pattern.at({0, 0, 0}));
                ASSERT_EQ_MSG("{0.25, 0, 0}", Color(0.75, 0.75, 0.75), pattern.at({0.25, 0, 0}));
                ASSERT_EQ_MSG("{0.5, 0, 0}", Color(0.5, 0.5, 0.5), pattern.at({0.5, 0, 0}));
                ASSERT_EQ_MSG("{0.75, 0, 0}", Color(0.25, 0.25, 0.25), pattern.at({0.75, 0, 0}));
            });
        });
    }
}