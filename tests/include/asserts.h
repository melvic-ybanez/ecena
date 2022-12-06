//
// Created by Melvic Ybanez on 12/5/22.
//

#ifndef RAYTRACER_ASSERTS_H
#define RAYTRACER_ASSERTS_H

#include <string>
#include <iostream>

void assert_true(const std::string &message, bool cond);

void assert_function(const std::string &message, const std::function<bool()> &pred);

#endif //RAYTRACER_ASSERTS_H
