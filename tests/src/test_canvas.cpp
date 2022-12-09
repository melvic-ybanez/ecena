//
// Created by Melvic Ybanez on 12/10/22.
//

#include "../include/test_canvas.h"
#include "../include/asserts.h"
#include "../../include/canvas.h"
#include "../../include/tuples.h"

namespace tests::canvas {
    void all() {
        set("Creating a Canvas", [] {
            rt::Canvas canvas{10, 20};
            ASSERT_EQ_MSG("Width", canvas.width(), 10);
            ASSERT_EQ_MSG("Height", canvas.height(), 20);

            bool all_black = true;
            for (const auto &row : canvas.pixels()) {
                for (const auto &pixel : row) {
                    if (pixel != rt::Color::black) {
                        all_black = false;
                        break;
                    }
                }
                if (!all_black) break;
            }
            ASSERT_TRUE("Every pixel is black", all_black);
        });
    }
}