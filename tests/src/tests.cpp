//
// Created by Melvic Ybanez on 12/7/22.
//

#include <iostream>
#include "../include/tests.h"
#include "../include/asserts.h"

int main() {
    using namespace rt::tests;

    reset();

    tuples();
    canvas();
    matrices();
    transformations();
    rays();
    spheres();
    intersections();
    lights();
    materials();
    world();
    camera();
    shapes();
    planes();
    patterns();

    std::cout << "Ran " << test_count << " tests. Successful: " << success_count << ". Failed: "
              << test_count - success_count << ".";
}