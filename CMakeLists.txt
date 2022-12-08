cmake_minimum_required(VERSION 3.24)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracer main.cpp include/image.h include/tuples.h include/color.h src/color.cpp include/ray.h src/ray.cpp src/tuples.cpp)
add_executable(raytracer_tests tests/src/tests.cpp tests/include/asserts.h tests/src/asserts.cpp tests/include/test_tuples.h tests/src/test_tuples.cpp src/tuples.cpp)
