//
// Created by Melvic Ybanez on 1/1/23.
//

#include <cmath>
#include "../include/camera.h"

namespace rt {
    Camera::Camera(real hsize, real vsize, real field_of_view)
            : hsize{hsize}, vsize{vsize},
              field_of_view{field_of_view},
              transform{math::matrix::identity<4, 4>()} {}

    real Camera::half_view() const {
        // splitting the field of view in half gives us a right triangle
        return std::tan(field_of_view / 2);
    }

    real Camera::pixel_size() const {
        return half_width() * 2 / hsize;
    }

    real Camera::aspect_ratio() const {
        return hsize / vsize;
    }

    real Camera::half_width() const {
        if (aspect_ratio() >= 1) {
            return half_view();
        }
        return half_view() * aspect_ratio();
    }

    real Camera::half_height() const {
        if (aspect_ratio() >= 1) {
            return half_view() / aspect_ratio();
        }
        return half_view();
    }

    Ray Camera::ray_for_pixel(int x, int y) const {
        auto pixel_size = this->pixel_size();

        // offsets from the edges of the canvas to the center of the pixel
        auto x_offset = (x + 0.5) * pixel_size;
        auto y_offset = (y + 0.5) * pixel_size;

        // world space coordinates
        auto world_x = half_width() - x_offset;     // +x is left because camera looks toward -z
        auto world_y = half_height() - y_offset;

        auto transform_inverse = transform.inverse();
        auto canvas_point = Point{world_x, world_y, -1};
        auto pixel = transform_inverse * canvas_point;
        auto origin = Point{transform_inverse * Point{0, 0, 0}};
        auto direction = Vec(pixel - origin).normalize();

        return {origin, direction};
    }
}