//
// Created by Melvic Ybanez on 12/10/22.
//

#include "../include/test_canvas.h"
#include "../include/asserts.h"
#include "../../include/canvas.h"
#include "../../include/tuples.h"
#include "../../include/ppm.h"

namespace tests::canvas {
    void all() {
        set("Canvas", []() {
            init();
            access();
            ppm();
        });
    }

    void init() {
        set("Creating a Canvas", [] {
            rt::Canvas canvas{10, 20};

            ASSERT_EQ_MSG("Width", canvas.width(), 10);
            ASSERT_EQ_MSG("Height", canvas.height(), 20);
            ASSERT_EQ_MSG("Canvas height is the number of rows", canvas.height(), canvas.pixels().size());
            ASSERT_EQ_MSG("Canvas width is the number of columns", canvas.width(), canvas.pixels()[0].size());

            bool all_black = true;
            for (const auto &row: canvas.pixels()) {
                for (const auto &pixel: row) {
                    if (pixel != rt::Color::black_) {
                        all_black = false;
                        break;
                    }
                }
                if (!all_black) break;
            }

            ASSERT_TRUE("Every pixel is black", all_black);
        });
    }

    void access() {
        scenario("Accessing pixels", []() {
            rt::Canvas canvas{10, 20};
            rt::Color color{rt::Color::red_};
            canvas.write_pixel(2, 3, color);

            ASSERT_EQ(canvas.pixel_at(2, 3), color);
        });
    }

    void ppm() {
        set("PPM", []() {
            scenario("Constructing the header", []() {
                rt::Canvas canvas{5, 3};
                rt::Ppm ppm{canvas};
                ASSERT_EQ(ppm.header(), "P3\n5 3\n255");
            });
            scenario("Constructing the pixel data", []() {
                rt::Canvas canvas{5, 3};

                rt::Color c1{1.5, 0, 0};
                rt::Color c2{0, 0.5, 0};
                rt::Color c3{-0.5, 0, 1};

                canvas.write_pixel(0, 0, c1);
                canvas.write_pixel(1, 2, c2);
                canvas.write_pixel(2, 4, c3);

                rt::Ppm ppm{canvas};

                ASSERT_EQ("255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                          "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n"
                          "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255\n",
                          ppm.pixel_data());
            });
            scenario("Splitting long lines", []() {
                rt::Canvas canvas{10, 2};
                for (int r = 0; r < 2; r++) {
                    for (int c = 0; c < 10; c++) {
                        canvas.write_pixel(r, c, rt::Color{1, 0.8, 0.6});
                    }
                }
                rt::Ppm ppm{canvas};

                ASSERT_EQ("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                          "153 255 204 153 255 204 153 255 204 153 255 204 153\n"
                          "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
                          "153 255 204 153 255 204 153 255 204 153 255 204 153\n",
                          ppm.pixel_data());
            });
            scenario("PPM files are terminated by a newline character", []() {
                rt::Canvas canvas{5, 3};
                rt::Ppm ppm{canvas};
                ASSERT_EQ('\n', ppm.pixel_data().back());
            });
        });
    }
}