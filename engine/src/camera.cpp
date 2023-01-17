//
// Created by Melvic Ybanez on 1/1/23.
//

#include <cmath>
#include "../include/camera.h"

namespace rt {
    Camera::Camera(double h_size, double v_size, real field_of_view)
            : h_size{h_size}, v_size{v_size},
              field_of_view{field_of_view},
              transform{math::matrix::identity<4, 4>()},
              antialias{false}, bg_colors{Color::black_} {}

    Camera::Camera() : Camera(0, 0, 0) {}

    real Camera::half_view() const {
        // splitting the field of view in half gives us a right triangle
        return std::tan(field_of_view / 2);
    }

    real Camera::pixel_size() const {
        return half_width() * 2 / h_size;
    }

    real Camera::aspect_ratio() const {
        return h_size / v_size;
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
        auto origin = transform_inverse * Point{0, 0, 0};
        auto direction = Vec(pixel - origin).normalize();

        return {origin, direction};
    }

    Canvas Camera::render(const World &world) const {
        Canvas canvas{static_cast<int>(h_size), static_cast<int>(v_size)};

        if (antialias) {
            int scale = 2;

            // set to higher resolution first
            auto high_res_cam = *this;
            high_res_cam.h_size *= scale;
            high_res_cam.v_size *= scale;
            high_res_cam.antialias = false;
            auto high_res = high_res_cam.render(world);

            // apply down-sampling to remove jagged edges
            for (auto y = 0; y < canvas.height(); y++) {
                auto source_y = y * scale;
                for (auto x = 0; x < canvas.width(); x++) {
                    auto source_x = x * scale;
                    auto average = (high_res[source_y][source_x] + high_res[source_y + 1][source_x]
                            + high_res[source_y][source_x + 1] + high_res[source_y + 1][source_x + 1]) * 0.25;
                    canvas[y][x] = average;
                }
            }

            return canvas;
        }

        for (auto y = 0; y < v_size; y++) {
            for (auto x = 0; x < h_size; x++) {
                auto ray = ray_for_pixel(x, y);

                auto bg_color = Color::black_;
                if (bg_colors.index() == 0) bg_color = std::get<Color>(bg_colors);
                if (bg_colors.index() == 1) {
                    auto colors = std::get<std::pair<Color, Color>>(bg_colors);

                    // performs linear interpolation
                    real t = ((v_size - y) / v_size + 1.0) * 0.5;
                    bg_color = colors.first * (1.0 - t) + colors.second * t;
                }

                auto color = world.color_at(ray, World::default_depth_, bg_color);
                canvas[y][x] = color;
            }
        }

        return canvas;
    }

    std::ostream &operator<<(std::ostream &out, const Camera &camera) {
        return out << "{ h_size: " << camera.h_size << ", v_size: " << camera.v_size
                   << ", field_of_view: " << camera.field_of_view << ", transform: "
                   << camera.transform << " }";
    }
}