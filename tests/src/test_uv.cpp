//
// Created by Melvic Ybanez on 2/16/23.
//

#include <cmath>
#include <array>

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/math/include/tuples.h"
#include "../../engine/include/uv.h"

namespace rt::tests::uv_ {
    static void spherical_map();

    static void image_based();

    void test() {
        set("UV", [] {
            spherical_map();
            image_based();
        });
    }

    void spherical_map() {
        set("Using a spherical mapping on a 3D point", [] {
            std::array<Point, 7> points{
                    {{0, 0, -1}, {1, 0, 0}, {0, 0, 1}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0},
                     {std::sqrt(2) / 2, std::sqrt(2) / 2, 0}}
            };
            std::array<real, 7> us{0.0, 0.25, 0.5, 0.75, 0.5, 0.5, 0.25};
            std::array<real, 7> vs{0.5, 0.5, 0.5, 0.5, 1.0, 0.0, 0.75};

            for (int i = 0; i < 7; i++) {
                auto point = points[i];
                auto [u, v] = uv::maps::spherical(point);

                ASSERT_EQ_MSG(std::to_string(i) + " - U", us[i], u);
                ASSERT_EQ_MSG(std::to_string(i) + " - V", vs[i], v);
            }
        });
    }

    void image_based() {
        set("Checker pattern in 2D", [] {
            std::stringstream ppm{
                    "P3\n"
                    "10 10\n"
                    "10\n"
                    "0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9\n"
                    "1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0\n"
                    "2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1\n"
                    "3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2\n"
                    "4 4 4  5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3\n"
                    "5 5 5  6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4\n"
                    "6 6 6  7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5\n"
                    "7 7 7  8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6\n"
                    "8 8 8  9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7\n"
                    "9 9 9  0 0 0  1 1 1  2 2 2  3 3 3  4 4 4  5 5 5  6 6 6  7 7 7  8 8 8\n"
            };
            auto canvas = canvas::from_ppm(ppm);
            auto pattern = std::make_unique<uv::Image>(canvas.value());
            const auto size = 4;
            std::array<real, size> us{0, 0.3, 0.6, 1};
            std::array<real, size> vs{0, 0, 0.3, 1};
            std::array<Color, size> expected{
                    {{0.9, 0.9, 0.9}, {0.2, 0.2, 0.2}, {0.1, 0.1, 0.1}, {0.9, 0.9, 0.9}}
            };

            for (int i = 0; i < size; i++) {
                auto color = pattern->at(us[i], vs[i]);
                ASSERT_EQ_MSG(std::to_string(i) + " - Color", expected[i], color);
            }
        });
    }
}