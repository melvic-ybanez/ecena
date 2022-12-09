//
// Created by Melvic Ybanez on 12/5/22.
//

#ifndef RAYTRACER_ASSERTS_H
#define RAYTRACER_ASSERTS_H

#include <string>
#include <iostream>
#include <sstream>

#define ASSERT_TRUE(message, cond) assert_true(message, cond, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_EQ_MSG(message, a, b) assert_equals(message, a, b, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_EQ(a, b) assert_equals(a, b, __PRETTY_FUNCTION__, __LINE__)

namespace tests {
    extern int test_count;
    extern int success_count;
    extern int set_level;
    extern std::string scenario_msg;

    void reset();

    void assert_true(const std::string &message, bool cond, const char *function_name, int line_no);

    template<typename T1, typename T2>
    void assert_equals(const std::string &message, T1 t1, T2 t2, const char *function_name, int line_no) {
        std::ostringstream msg_stream;
        bool cond = t1 == t2;
        if (cond) {
            msg_stream << message;
        } else {
            msg_stream << message << ". Expected: " << t1 << ". Got: " << t2;
        }
        assert_true(msg_stream.str(), t1 == t2, function_name, line_no);
    }

    template<typename T1, typename T2>
    void assert_equals(T1 t1, T2 t2, const char *function_name, int line_no) {
        assert_equals(scenario_msg, t1, t2, function_name, line_no);
    }

    void scenario(const std::string &message, const std::function<void()> &f);

    void set(const std::string &message, const std::function<void()> &f);

    void indent_set();
}

#endif //RAYTRACER_ASSERTS_H
