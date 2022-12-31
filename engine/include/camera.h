//
// Created by Melvic Ybanez on 1/1/23.
//

#ifndef ECENA_CAMERA_H
#define ECENA_CAMERA_H

#include "../math/include/math_utils.h"
#include "../math/include/matrix.h"

namespace rt {
    class Camera {
    public:
        const real hsize;
        const real vsize;
        const real field_of_view;     // how much the camera can see
        const math::Matrix<4, 4> transform;

        Camera(real hsize, real vsize, real field_of_view);

        real pixel_size() const;

        real half_width() const;

        real half_height() const;

        real aspect_ratio() const;

        real half_view() const;
    };
}

#endif //ECENA_CAMERA_H
