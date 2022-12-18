//
// Created by Melvic Ybanez on 12/7/22.
//

#include <iostream>
#include "../include/test_tuples.h"
#include "../include/asserts.h"
#include "../include/test_canvas.h"
#include "../include/test_matrices.h"
#include "../include/test_transformations.h"
#include "../include/test_rays.h"
#include "../include/test_spheres.h"

int main() {
    tests::reset();
    tests::tuples::all();
    tests::canvas::all();
    tests::matrices::all();
    tests::transformations::all();
    tests::rays::all();
    tests::spheres::all();
    std::cout << "Ran " << tests::test_count << " tests. Successful: " << tests::success_count << ". Failed: "
              << tests::test_count - tests::success_count << ".";
}