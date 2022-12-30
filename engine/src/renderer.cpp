//
// Created by Melvic Ybanez on 12/26/22.
//

#include "../include/renderer.h"

namespace rt {
    Ppm render(Data &data) {
        // for now, let's assume the use is always looking at the center of the scene
        math::Dimensions half{data.wall.size.width / 2, data.wall.size.height / 2};

        for (int y = 0; y < data.canvas.height(); y++) {
            real world_y = half.height - data.pixel_size().height * y;
            for (int x = 0; x < data.canvas.width(); x++) {
                real world_x = -half.width + data.pixel_size().width * x;
                auto position = Point{world_x, world_y, data.wall.location.z()};
                Ray ray{data.ray_origin, Vec(position - data.ray_origin).normalize()};

                for (auto &shape : data.shapes) {
                    auto xs = shape->intersect(ray);
                    auto hit = xs.hit();
                    if (hit != nullptr) {
                        auto point = ray.at(hit->t);
                        auto normal = hit->object->normal_at(point);
                        auto eye_vec = Vec(-ray.direction);
                        auto color = lights::lighting(hit->object->material, data.light, point, eye_vec, normal);
                        data.canvas[y][x] = color;
                    }
                }
            }
        }

        return Ppm{data.canvas};
    }
}