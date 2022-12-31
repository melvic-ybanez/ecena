//
// Created by Melvic Ybanez on 1/1/23.
//

#include <cmath>
#include "../include/test_camera.h"
#include "../include/asserts.h"
#include "../../engine/include/camera.h"
#include "../include/test_utils.h"

namespace rt::tests::camera {
    void all() {
        set("Camera", [] {
            init();
            pixel_size();
            rays();
            rendering();
        });
    }

    void init() {
        set("Constructing a camera", [] {
            Camera camera{160, 120, math::pi / 2};
            auto default_transform = matrix::identity<4, 4>();

            ASSERT_EQ_MSG("horizontal size", 160, camera.h_size);
            ASSERT_EQ_MSG("vertical size", 120, camera.v_size);
            ASSERT_EQ_MSG("field of view", math::pi / 2, camera.field_of_view);
            ASSERT_EQ_MSG("transform", default_transform, camera.transform);
        });
    }

    void pixel_size() {
        scenario("The pixel size for a horizontal canvas", [] {
            Camera camera{200, 125, math::pi / 2};
            ASSERT_TRUE(math::compare_reals(0.01, camera.pixel_size()));
        });
        scenario("The pixel size for a vertical canvas", [] {
            Camera camera{125, 200, math::pi / 2};
            ASSERT_TRUE(math::compare_reals(0.01, camera.pixel_size()));
        });
    }

    void rays() {
        set("Rays for pixels", [] {
            scenario("Constructing a ray through the center of the canvas", [] {
                Camera camera{201, 101, math::pi / 2};
                auto ray = camera.ray_for_pixel(100, 50);
                Ray expected{{0, 0, 0},
                             {0, 0, -1}};
                ASSERT_EQ(expected, ray);
            });
            scenario("Constructing a ray through a corner of the canvas", [] {
                Camera camera{201, 101, math::pi / 2};
                auto ray = camera.ray_for_pixel(0, 0);
                Ray expected{{0,       0,       0},
                             {0.66519, 0.33259, -0.66851}};
                ASSERT_EQ(expected, ray);
            });
            scenario("Constructing a ray when the camera is transformed", [] {
                Camera camera{201, 101, math::pi / 2};
                camera.transform = matrix::rotation_y(math::pi / 4) * matrix::translation(0, -2, 5);
                auto ray = camera.ray_for_pixel(100, 50);
                Ray expected{{0,                2, -5},
                             {std::sqrt(2) / 2, 0, -std::sqrt(2) / 2}};
                ASSERT_EQ(expected, ray);
            });
        });
    }

    void rendering() {
        scenario("Rendering a world with a camera", [] {
            auto world = default_world();
            Camera camera{11, 11, math::pi / 2};
            camera.transform = matrix::view_transform({0, 0, -5}, {0, 0, 0}, {0, 1, 0});
            auto image = camera.render(world);
            ASSERT_EQ(Color(0.38066, 0.47583, 0.2855), image[5][5]);
        });
    }
}