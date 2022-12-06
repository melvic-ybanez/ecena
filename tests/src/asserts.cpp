//
// Created by Melvic Ybanez on 12/5/22.
//

#include "../include/asserts.h"

void assert_true(const std::string &message, bool cond) {
    if (cond) {
        std::cout << "[Successful] " << message << ".";
    } else {
        std::cout << "[Failed] " << message << "." << std::endl;
        std::cout << "at " << __FUNCTION__ << ", line " << __LINE__;
    }
}

void assert_function(const std::string &message, const std::function<bool()> &pred) {
    assert_true(message, pred());
}