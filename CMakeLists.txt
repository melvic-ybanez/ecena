cmake_minimum_required(VERSION 3.24)
project(ecena)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        ecena main.cpp
        math/include/tuples.h
        math/src/tuples.cpp
        math/include/math_utils.h
        math/src/math_utils.cpp
        core/include/canvas.h
        core/src/canvas.cpp
        core/include/ppm.h
        core/src/ppm.cpp
        math/include/matrix.h
        math/src/matrix.cpp
        core/include/ray.h
        core/src/ray.cpp
        core/include/shapes.h
        core/include/intersection.h
        core/src/shapes.cpp
        core/src/intersection.cpp core/include/light.h core/include/material.h core/src/material.cpp core/src/light.cpp dsl/include/tokens.h dsl/include/lexer.h dsl/src/lexer.cpp dsl/include/errors.h dsl/src/errors.cpp dsl/include/parser.h dsl/include/ast.h dsl/src/parser.cpp dsl/src/ast.cpp core/include/renderer.h core/src/renderer.cpp core/include/data.h core/src/data.cpp dsl/include/eval.h dsl/src/eval.cpp)
add_executable(
        ecena_tests
        tests/src/tests.cpp
        tests/include/asserts.h
        tests/src/asserts.cpp
        tests/include/test_tuples.h
        tests/src/test_tuples.cpp
        math/src/tuples.cpp
        math/include/math_utils.h
        math/src/math_utils.cpp
        core/include/canvas.h
        core/src/canvas.cpp
        tests/include/test_canvas.h
        tests/src/test_canvas.cpp
        core/include/ppm.h
        core/src/ppm.cpp
        math/include/matrix.h
        math/src/matrix.cpp
        tests/src/test_matrices.cpp
        tests/include/test_matrices.h
        tests/include/test_transformations.h
        tests/src/test_transformations.cpp
        core/include/ray.h
        core/src/ray.cpp
        tests/include/test_rays.h
        tests/src/test_rays.cpp
        tests/include/test_spheres.h
        tests/src/test_spheres.cpp core/include/shapes.h core/include/intersection.h tests/include/test_intersections.h tests/src/test_intersections.cpp core/src/shapes.cpp core/src/intersection.cpp core/include/light.h tests/include/test_lights.h tests/src/test_lights.cpp core/include/material.h tests/include/test_materials.h tests/src/test_materials.cpp core/src/material.cpp core/src/light.cpp dsl/include/tokens.h dsl/include/lexer.h dsl/src/lexer.cpp dsl/include/errors.h dsl/src/errors.cpp dsl/include/parser.h dsl/include/ast.h dsl/src/parser.cpp dsl/src/ast.cpp)
