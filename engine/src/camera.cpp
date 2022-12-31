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
}