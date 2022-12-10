cmake_minimum_required(VERSION 3.24)
project(ecena)

set(CMAKE_CXX_STANDARD 17)

add_executable(ecena main.cpp include/tuples.h src/tuples.cpp include/math_utils.h src/math_utils.cpp include/canvas.h src/canvas.cpp include/ppm.h src/ppm.cpp)
add_executable(ecena_tests tests/src/tests.cpp tests/include/asserts.h tests/src/asserts.cpp tests/include/test_tuples.h tests/src/test_tuples.cpp src/tuples.cpp include/math_utils.h src/math_utils.cpp include/canvas.h src/canvas.cpp tests/include/test_canvas.h tests/src/test_canvas.cpp include/ppm.h src/ppm.cpp)
