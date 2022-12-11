cmake_minimum_required(VERSION 3.24)
project(ecena)

set(CMAKE_CXX_STANDARD 17)

add_executable(ecena main.cpp core/include/tuples.h core/src/tuples.cpp math/include/utils.h math/src/utils.cpp core/include/canvas.h core/src/canvas.cpp core/include/ppm.h core/src/ppm.cpp math/include/matrix.h tests/src/test_matrices.cpp tests/include/test_matrices.h math/src/matrix.cpp)
add_executable(ecena_tests tests/src/tests.cpp tests/include/asserts.h tests/src/asserts.cpp tests/include/test_tuples.h tests/src/test_tuples.cpp core/src/tuples.cpp math/include/utils.h math/src/utils.cpp core/include/canvas.h core/src/canvas.cpp tests/include/test_canvas.h tests/src/test_canvas.cpp core/include/ppm.h core/src/ppm.cpp math/include/matrix.h tests/src/test_matrices.cpp tests/include/test_matrices.h math/src/matrix.cpp)
