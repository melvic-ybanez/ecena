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
        bool antialias;
        std::variant<Color, std::pair<Color, Color>> bg_colors;

        Camera();

        Camera(double h_size, double v_size, real field_of_view);

        [[nodiscard]] real pixel_size() const;

        [[nodiscard]] real half_width() const;

        [[nodiscard]] real half_height() const;

        [[nodiscard]] real aspect_ratio() const;

        [[nodiscard]] real half_view() const;

        [[nodiscard]] Ray ray_for_pixel(int x, int y) const;

        [[nodiscard]] Canvas render(const World& world) const;
    };

    std::ostream& operator<<(std::ostream& out, const Camera& camera);
}

#endif //ECENA_CAMERA_H
