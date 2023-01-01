//
// Created by Melvic Ybanez on 1/1/23.
//

#ifndef ECENA_CAMERA_H
#define ECENA_CAMERA_H

#include "../math/include/math_utils.h"
#include "../math/include/matrix.h"
#include "ray.h"
#include "canvas.h"
#include "world.h"

namespace rt {
    class Camera {
    public:
        double h_size;
        double v_size;
        real field_of_view;     // how much the camera can see
        Matrix<4, 4> transform;

        Camera();

        Camera(double h_size, double v_size, real field_of_view);

        real pixel_size() const;

        real half_width() const;

        real half_height() const;

        real aspect_ratio() const;

        real half_view() const;

        Ray ray_for_pixel(int x, int y) const;

        Canvas render(const World &world) const;
    };

    std::ostream &operator<<(std::ostream &out, const Camera &camera);
}

#endif //ECENA_CAMERA_H
