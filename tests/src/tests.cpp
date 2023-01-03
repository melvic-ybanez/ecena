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
#include "../include/test_shapes.h"

int main() {
    using namespace rt::tests;

    reset();
    tuples::all();
    canvas::all();
    matrices::all();
    transformations::all();
    rays::all();
    spheres::all();
    intersections::all();
    lights::all();
    materials::all();
    world::all();
    camera::all();
    shapes::all();
    std::cout << "Ran " << test_count << " tests. Successful: " << success_count << ". Failed: "
              << test_count - success_count << ".";
}