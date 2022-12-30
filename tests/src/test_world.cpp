//
// Created by Melvic Ybanez on 12/30/22.
//

#include "../include/test_world.h"
#include "../include/asserts.h"
#include "../../engine/include/world.h"

namespace tests::world {
    void all() {
        set("World", [] {
            init();
        });
    }

    void init() {
        set("Creating a world", [] {
           rt::World world;
            ASSERT_EQ_MSG("Inspect objects", 0, world.objects.size());
            ASSERT_TRUE_MSG("Inspect light source", !world.light.has_value());
        });
    }
}