//
// Created by Melvic Ybanez on 12/5/22.
//

#include "../include/asserts.h"

namespace tests {
    int test_count = 0;
    int success_count = 0;

    void assert_true(const std::string &message, bool cond) {
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

    void scenario(const std::string &message, const std::function<bool()> &pred) {
        assert_true(message, pred());
    }
}