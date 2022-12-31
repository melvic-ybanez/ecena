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
#include "../include/test_intersections.h"
#include "../include/test_lights.h"
#include "../include/test_materials.h"
#include "../include/test_world.h"
#include "../include/test_camera.h"

int main() {
    tests::reset();
    tests::tuples::all();
    tests::canvas::all();
    tests::matrices::all();
    tests::transformations::all();
    tests::rays::all();
    tests::spheres::all();
    tests::intersections::all();
    tests::lights::all();
    tests::materials::all();
    tests::world::all();
    tests::camera::all();
    std::cout << "Ran " << tests::test_count << " tests. Successful: " << tests::success_count << ". Failed: "
              << tests::test_count - tests::success_count << ".";
}