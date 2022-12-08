//
// Created by Melvic Ybanez on 12/5/22.
//

#include "../include/asserts.h"

namespace tests {
    int test_count;
    int success_count;
    int set_level;
    std::string scenario_msg;

    void reset() {
        test_count = 0;
        success_count = 0;
        set_level = 0;
        scenario_msg = "";
    }

    void assert_true(const std::string &message, bool cond, const char *function_name, int line_no) {
        indent_set();
        std::cout << "- ";

        if (cond) {
            std::cout << "[Successful] " << message;
            success_count++;
        } else {
            std::cout << "[Failed] " << message << ", " << std::endl;
            indent_set();
            std::cout << "  at " << function_name << ", line " << line_no;
        }

        test_count++;
        std::cout << std::endl;
    }

    void scenario(const std::string &message, const std::function<void()> &f) {
        scenario_msg = message;
        f();
        scenario_msg = "";
    }

    void set(const std::string &message, const std::function<void()> &f) {
        indent_set();
        std::cout << "- " << message << std::endl;

        set_level++;
        f();
        set_level--;
    }

    void indent_set() {
        for (int i = 0; i < set_level; i++) {
            std::cout << "  ";
        }
    }
}