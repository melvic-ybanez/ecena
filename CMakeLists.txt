cmake_minimum_required(VERSION 3.24)
project(ecena)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        ecena main.cpp
        engine/math/include/tuples.h
        engine/math/src/tuples.cpp
        engine/math/include/math_utils.h
        engine/math/src/math_utils.cpp
        engine/include/canvas.h
        engine/src/canvas.cpp
        engine/include/ppm.h
        engine/src/ppm.cpp
        engine/math/include/matrix.h
        engine/math/src/matrix.cpp
        engine/include/ray.h
        engine/src/ray.cpp
        engine/include/shapes.h
        engine/include/intersection.h
        engine/src/shapes.cpp
        engine/src/intersection.cpp engine/include/light.h engine/include/materials.h engine/src/materials.cpp engine/src/light.cpp dsl/include/tokens.h dsl/include/lexer.h dsl/src/lexer.cpp dsl/include/errors.h dsl/src/errors.cpp dsl/include/parser.h dsl/include/ast.h dsl/src/parser.cpp dsl/src/ast.cpp engine/include/renderer.h engine/src/renderer.cpp engine/include/data.h engine/src/data.cpp dsl/include/eval.h dsl/src/eval.cpp shared/include/utils.h engine/include/world.h engine/src/world.cpp engine/include/comps.h engine/src/comps.cpp engine/include/camera.h engine/src/camera.cpp engine/include/patterns.h engine/src/patterns.cpp engine/math/include/transform.h)
add_executable(
        ecena_tests
        tests/src/tests.cpp
        tests/include/asserts.h
        tests/src/asserts.cpp
        tests/src/test_tuples.cpp
        engine/math/src/tuples.cpp
        engine/math/include/math_utils.h
        engine/math/src/math_utils.cpp
        engine/include/canvas.h
        engine/src/canvas.cpp
        tests/src/test_canvas.cpp
        engine/include/ppm.h
        engine/src/ppm.cpp
        engine/math/include/matrix.h
        engine/math/src/matrix.cpp
        tests/src/test_matrices.cpp
        tests/src/test_transformations.cpp
        engine/include/ray.h
        engine/src/ray.cpp
        tests/src/test_rays.cpp
        tests/src/test_spheres.cpp engine/include/shapes.h engine/include/intersection.h tests/src/test_intersections.cpp engine/src/shapes.cpp engine/src/intersection.cpp engine/include/light.h tests/src/test_lights.cpp engine/include/materials.h tests/src/test_materials.cpp engine/src/materials.cpp engine/src/light.cpp dsl/include/tokens.h dsl/include/lexer.h dsl/src/lexer.cpp dsl/include/errors.h dsl/src/errors.cpp dsl/include/parser.h dsl/include/ast.h dsl/src/parser.cpp dsl/src/ast.cpp engine/include/world.h tests/src/test_world.cpp engine/src/world.cpp engine/include/comps.h engine/src/comps.cpp engine/include/camera.h tests/src/test_camera.cpp engine/src/camera.cpp tests/include/test_utils.h tests/src/test_utils.cpp tests/src/test_shapes.cpp tests/src/test_planes.cpp tests/include/tests.h tests/src/test_patterns.cpp engine/include/patterns.h engine/src/patterns.cpp engine/math/include/transform.h)
