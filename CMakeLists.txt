cmake_minimum_required(VERSION 3.24)
project(ecena)

set(CMAKE_CXX_STANDARD 17)

add_executable(ecena main.cpp include/image.h include/tuples.h src/tuples.cpp include/rtmath.h src/rtmath.cpp)
add_executable(ecena_tests tests/src/tests.cpp tests/include/asserts.h tests/src/asserts.cpp tests/include/test_tuples.h tests/src/test_tuples.cpp src/tuples.cpp include/rtmath.h src/rtmath.cpp)
