//
// Created by Melvic Ybanez on 12/7/22.
//

#include <iostream>
#include "../include/tests.h"
#include "../include/asserts.h"

int main() {
    using namespace rt::tests;

    reset();

    tuples::test();
    canvas::test();
    matrices::test();
    transformations::test();
    rays::test();
    spheres::test();
    intersections::test();
    lights_::test();
    materials::test();
    world::test();
    camera::test();
    shapes_::test();
    planes::test();
    patterns_::test();
    cubes::test();
    cylinders::test();

    std::cout << "Ran " << test_count << " tests. Successful: " << success_count << ". Failed: "
              << test_count - success_count << ".";
}