//
// Created by Melvic Ybanez on 12/5/22.
//

#ifndef RAYTRACER_ASSERTS_H
#define RAYTRACER_ASSERTS_H

#include <string>
#include <iostream>
#include <sstream>

namespace tests {
    extern int test_count;
    extern int success_count;
    extern int scenario_level;

    void reset_counts();

    void assert_true(const std::string &message, bool cond);

    /**
     * Performs an assertion from a function.
     * Note: I decided not to make the function parameter map to another function that takes the message and
     * use a variant of `assert_equals` that aligns with the returned signature (which
     * could have made the usage more ergonomic).
     */
    void assert_with(const std::string &message, const std::function<void(const std::string &)> &f);

    template<typename T1, typename T2>
    void assert_equals(const std::string &message, T1 t1, T2 t2) {
        std::ostringstream msg_stream;
        bool cond = t1 == t2;
        if (cond) {
            msg_stream << message;
        } else {
            msg_stream << message << ". Expected: " << t1 << ". Got: " << t2;
        }
        assert_true(msg_stream.str(), t1 == t2);
    }

    void set(const std::string &message, const std::function<void()> &f);
}

#endif //RAYTRACER_ASSERTS_H
