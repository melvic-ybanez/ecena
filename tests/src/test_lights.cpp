//
// Created by Melvic Ybanez on 12/23/22.
//

#include "../include/test_lights.h"
#include "../include/asserts.h"
#include "../../math/include/tuples.h"
#include "../../core/include/light.h"

namespace tests::lights {
    void all() {
        set("Lights", [] {
            init();
        });
    }

    void init() {
        set("A point light has a position and intensity", [] {
            rt::Color intensity{1, 1, 1};
            rt::Point position{0, 0, 0};
            rt::PointLight light{position, intensity};

            ASSERT_EQ_MSG("Inspect position", position, light.position);
            ASSERT_EQ_MSG("Inspect intensity", intensity, light.intensity);
        });
    }
}