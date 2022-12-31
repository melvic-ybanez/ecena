//
// Created by Melvic Ybanez on 1/1/23.
//

#include "../include/test_camera.h"
#include "../include/asserts.h"
#include "../../engine/include/camera.h"

namespace tests::camera {
    void all() {
        set("Camera", [] {
            init();
            pixel_size();
        });
    }

    void init() {
        set("Constructing a camera", [] {
            rt::Camera camera{160, 120, rt::math::pi / 2};
            auto default_transform = rt::math::matrix::identity<4, 4>();

            ASSERT_EQ_MSG("horizontal size", 160, camera.hsize);
            ASSERT_EQ_MSG("vertical size", 120, camera.vsize);
            ASSERT_EQ_MSG("field of view", rt::math::pi / 2, camera.field_of_view);
            ASSERT_EQ_MSG("transform", default_transform, camera.transform);
        });
    }

    void pixel_size() {
        scenario("The pixel size for a horizontal canvas", [] {
            rt::Camera camera{200, 125, rt::math::pi / 2};
            ASSERT_TRUE(rt::math::compare_reals(0.01, camera.pixel_size()));
        });
        scenario("The pixel size for a vertical canvas", [] {
            rt::Camera camera{125, 200, rt::math::pi / 2};
            ASSERT_TRUE(rt::math::compare_reals(0.01, camera.pixel_size()));
        });
    }
}