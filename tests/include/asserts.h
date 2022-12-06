//
// Created by Melvic Ybanez on 12/5/22.
//

#ifndef RAYTRACER_ASSERTS_H
#define RAYTRACER_ASSERTS_H

#include <string>
#include <iostream>

namespace tests {
    extern int test_count;
    extern int success_count;

    void assert_true(const std::string &message, bool cond);

    void scenario(const std::string &message, const std::function<bool()> &pred);
}

#endif //RAYTRACER_ASSERTS_H
