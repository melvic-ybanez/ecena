//
// Created by Melvic Ybanez on 12/5/22.
//

#ifndef ECENA_ASSERTS_H
#define ECENA_ASSERTS_H

#include <string>
#include <iostream>
#include <sstream>

#define ASSERT_TRUE_MSG(message, cond) assert_true(message, cond, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_FALSE_MSG(message, cond) ASSERT_TRUE_MSG(message, !(cond))
#define ASSERT_TRUE(cond) ASSERT_TRUE_MSG(scenario_msg, cond)
#define ASSERT_FALSE(cond) ASSERT_FALSE_MSG(scenario_msg, cond)
#define ASSERT_EQ_MSG(message, a, b) assert_equals(message, a, b, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_EQ(a, b) ASSERT_EQ_MSG(scenario_msg, a, b)

namespace rt::tests {
    extern int test_count;
    extern int success_count;
    extern int set_level;
    extern std::string scenario_msg;

    void reset();

    void assert_true(const std::string &message, bool cond, const char *function_name, int line_no);

    template<typename T1, typename T2>
    void assert_equals(const std::string &message, const T1 &t1, const T2 &t2, const char *function_name, int line_no) {
        std::ostringstream msg_stream;
        bool cond = t1 == t2;
        if (cond) {
            msg_stream << message;
        } else {
            msg_stream << message << ". Expected: " << t1 << ". Got: " << t2;
        }
        assert_true(msg_stream.str(), t1 == t2, function_name, line_no);
    }

    /**
     * Like `assert_equals()`, but allows a group of statements inside a function `f`. The message parameter passed
     * will be available within the scope of `f`, and is automatically used in any `ASSERT_*` functions that
     * do not end with the `_MSG` suffix.
     */
    void scenario(const std::string &message, const std::function<void()> &f);

    void set(const std::string &message, const std::function<void()> &f);

    void indent_set();
}

#endif //ECENA_ASSERTS_H
