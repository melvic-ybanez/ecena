//
// Created by Melvic Ybanez on 12/23/22.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/light.h"

namespace rt::tests {
    static void init();

    void lights() {
        set("Lights", [] {
            init();
        });
    }

    void init() {
        set("A point light has a position and intensity", [] {
            rt::Color intensity{1, 1, 1};
            Point position{0, 0, 0};
            PointLight light{position, intensity};

            ASSERT_EQ_MSG("Inspect position", position, light.position);
            ASSERT_EQ_MSG("Inspect intensity", intensity, light.intensity);
        });
    }
}