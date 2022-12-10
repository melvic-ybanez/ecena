//
// Created by Melvic Ybanez on 12/10/22.
//

#include "../include/test_canvas.h"
#include "../include/asserts.h"
#include "../../include/canvas.h"
#include "../../include/tuples.h"

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
        scenario("Constructing the PPM header", []() {
            rt::Canvas canvas{5, 3};
            auto ppm = canvas.to_ppm();
            ASSERT_EQ(ppm.header(), "P3\n5 3\n255");
        });
    }
}