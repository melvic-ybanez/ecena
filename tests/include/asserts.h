//
// Created by Melvic Ybanez on 12/5/22.
//

#ifndef ECENA_ASSERTS_H
#define ECENA_ASSERTS_H

#include <string>
#include <iostream>
#include <sstream>
#include "../../engine/math/include/math_utils.h"

#define ASSERT_TRUE_MSG(message, cond) assert_true(message, cond, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_FALSE_MSG(message, cond) ASSERT_TRUE_MSG(message, !(cond))
#define ASSERT_TRUE(cond) ASSERT_TRUE_MSG(scenario_msg, cond)
#define ASSERT_FALSE(cond) ASSERT_FALSE_MSG(scenario_msg, cond)
#define ASSERT_EQ_MSG(message, a, b) assert_equals(message, a, b, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_NEQ_MSG(message, a, b) ASSERT_FALSE_MSG(message, a == b)
#define ASSERT_EQ(a, b) ASSERT_EQ_MSG(scenario_msg, a, b)
#define ASSERT_NEQ(a, b) ASSERT_NEQ_MSG(scenario_msg, a, b)
#define ASSERT_EQR_MSG(message, a, b) assert_equals_reals(message, a, b, __PRETTY_FUNCTION__, __LINE__)

namespace rt::tests {
    inline int test_count;
    inline int success_count;
    inline int set_level;
    inline std::string scenario_msg;

    void reset();

    void assert_true(const std::string& message, bool cond, const char* function_name, int line_no);

    template<typename T1, typename T2>
    void assert_equals(const std::string& message, const T1& t1, const T2& t2, const char* function_name, int line_no,
                       std::function<bool(const T1&, const T2&)> compare = [](const T1& t1, const T2& t2) {
                           return t1 == t2;
                       }) {
        std::ostringstream msg_stream;
        bool cond = compare(t1, t2);
        if (cond) {
            msg_stream << message;
        } else {
            msg_stream << message << ". Expected: " << t1 << ". Got: " << t2;
        }
        assert_true(msg_stream.str(), cond, function_name, line_no);
    }

    void assert_equals_reals(const std::string& message, real v1, real v2, const char* function_name, int line_no);

    /**
     * Like `assert_equals()`, but allows a group of statements inside a function `f`. The message parameter passed
     * will be available within the scope of `f`, and is automatically used in any `ASSERT_*` functions that
     * do not end with the `_MSG` suffix.
     */
    void scenario(const std::string& message, const std::function<void()>& f);

    void set(const std::string& message, const std::function<void()>& f);

    void indent_set();

}

#endif //ECENA_ASSERTS_H
