//
// Created by Melvic Ybanez on 12/5/22.
//

#include "../include/asserts.h"

namespace tests {
    int test_count;
    int success_count;
    int set_level;

    void reset_counts() {
        test_count = 0;
        success_count = 0;
        set_level = 0;
    }

    void assert_true(const std::string &message, bool cond) {
        indent_set();
        std::cout << "- ";

        if (cond) {
            std::cout << "[Successful] " << message << ".";
            success_count++;
        } else {
            std::cout << "[Failed] " << message << "." << std::endl;
            std::cout << "at " << __FUNCTION__ << ", line " << __LINE__;
        }

        test_count++;
        std::cout << std::endl;
    }

    void assert_with(const std::string &message, const std::function<void(const std::string &)> &f) {
        f(message);
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