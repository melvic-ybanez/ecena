//
// Created by Melvic Ybanez on 12/10/22.
//

#include <array>
#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/canvas.h"
#include "../../engine/include/ppm.h"

namespace rt::tests::canvas_ {
    static void init();

    static void access();

    static void write_to_ppm();

    static void read_from_ppm();

    void test() {
        set("Canvas", [] {
            init();
            access();
            write_to_ppm();
            read_from_ppm();
        });
    }

    void init() {
        set("Creating a Canvas", [] {
            rt::Canvas canvas{10, 20};

            ASSERT_EQ_MSG("Width", canvas.width(), 10);
            ASSERT_EQ_MSG("Height", canvas.height(), 20);
            ASSERT_EQ_MSG("Canvas height is the number of rows", canvas.height(), canvas.pixels.size());
            ASSERT_EQ_MSG("Canvas width is the number of columns", canvas.width(), canvas.pixels[0].size());

            bool all_black = true;
            for (const auto &row: canvas.pixels) {
                for (const auto &pixel: row) {
                    if (pixel != rt::Color::black_) {
                        all_black = false;
                        break;
                    }
                }
                if (!all_black) break;
            }

            ASSERT_TRUE_MSG("Every pixel is black", all_black);
        });
    }

    void access() {
        scenario("Accessing pixels", [] {
            rt::Canvas canvas{10, 20};
            rt::Color color{rt::Color::red_};
            canvas[2][3] = color;

            ASSERT_EQ(canvas[2][3], color);
        });
    }

    void write_to_ppm() {
        set("Write to PPM", [] {
            scenario("Constructing the header", [] {
                rt::Canvas canvas{5, 3};
                rt::Ppm ppm{canvas};
                ASSERT_EQ(ppm.header(), "P3\n5 3\n255");
            });
            scenario("Constructing the pixel data", [] {
                rt::Canvas canvas{5, 3};

                rt::Color c1{1.5, 0, 0};
                rt::Color c2{0, 0.5, 0};
                rt::Color c3{-0.5, 0, 1};

                canvas[0][0] = c1;
                canvas[1][2] = c2;
                canvas[2][4] = c3;

                rt::Ppm ppm{canvas};

                ASSERT_EQ("255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                          "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
                          "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n",
                          ppm.pixel_data());
            });
            scenario("Splitting long lines", [] {
                rt::Canvas canvas{10, 2};
                for (auto r = 0; r < 2; r++) {
                    for (auto c = 0; c < 10; c++) {
                        canvas[r][c] = rt::Color{1, 0.8, 0.6};
                    }
                }
                rt::Ppm ppm{canvas};

                ASSERT_EQ("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                          "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                          "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                          "153 255 204 153 255 204 153 255 204 153 255 204 153\n",
                          ppm.pixel_data());
            });
            scenario("PPM files are terminated by a newline character", [] {
                rt::Canvas canvas{5, 3};
                rt::Ppm ppm{canvas};
                ASSERT_EQ('\n', ppm.pixel_data().back());
            });
        });
    }

    void read_from_ppm() {
        set("Read from PPM", [] {
            scenario("Reading a file with the wrong magic number", [] {
                std::stringstream file{
                        "P32\n"
                        "1 1\n"
                        "255\n"
                        "0 0 0\n"
                };
                auto canvas = canvas::from_ppm(file);
                ASSERT_FALSE(canvas.has_value());
            });
            scenario("Reading a PPM returns a canvas of the right size", [] {
                std::stringstream ppm{
                        "P3\n"
                        "10 2\n"
                        "255\n"
                        "0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n"
                        "0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n"
                        "0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n"
                        "0 0 0  0 0 0  0 0 0  0 0 0  0 0 0\n"
                };
                auto canvas = canvas::from_ppm(ppm);
                ASSERT_EQ(math::Dimensions(10, 2), canvas.value().size);
            });
            set("Reading pixel data from a PPM file", [] {
                std::stringstream ppm{
                        "P3\n"
                        "4 3\n"
                        "255\n"
                        "255 127 0  0 127 255  127 255 0  255 255 255\n"
                        "0 0 0  255 0 0  0 255 0  0 0 255\n"
                        "255 255 0  0 255 255  255 0 255  127 127 127\n"
                };
                auto canvas = canvas::from_ppm(ppm);
                std::array<int, 12> xs{0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};
                std::array<int, 12> ys{0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2};
                std::array<Color, 12> colors{
                        {{1, 0.498, 0}, {0, 0.498, 1}, {0.498, 1, 0}, {1, 1, 1},
                         {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
                         {1, 1, 0}, {0, 1, 1}, {1, 0, 1}, {0.498, 0.498, 0.498}}
                };
                for (int i = 0; i < xs.size(); i++) {
                    auto color = canvas.value()[ys[i]][xs[i]];
                    ASSERT_EQ_MSG(std::to_string(i) + ":", colors[i], color);
                }
            });
            set("PPM parsing ignores comment lines", [] {
                std::stringstream ppm{
                        "P3\n"
                        "# this is a comment\n"
                        "2 1\n"
                        "# this, too\n"
                        "255\n"
                        "# another comment\n"
                        "255 255 255\n"
                        "# oh, no, comments in the pixel data!\n"
                        "255 0 255\n"
                };
                auto canvas = canvas::from_ppm(ppm);
                ASSERT_EQ_MSG("at 0 0", Color::white_, canvas.value().pixels[0][0]);
                ASSERT_EQ_MSG("at 1 0", Color(1, 0, 1), canvas.value().pixels[0][1]);
            });
        });
    }
}