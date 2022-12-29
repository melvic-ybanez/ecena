//
// Created by Melvic Ybanez on 12/26/22.
//

#include "../include/data.h"
#include "../../shared/include/utils.h"

namespace rt {
    Data::Data() {
        wall.size = {10, 10};
    }

    math::Dimensions Data::pixel_size() const {
        return {wall.size.width / canvas.width(), wall.size.height / canvas.height()};
    }

    std::ostream &operator<<(std::ostream &out, const Data &data) {
        return out << "{ ray_origin: " << data.ray_origin
                   << ", wall: " << data.wall << ", canvas: " << data.canvas
                   << ", pixel_size: " << data.pixel_size() << ", shapes: "
                   << join_to_array(data.shapes) << " }";
    }

    std::ostream &operator<<(std::ostream &out, const Wall &wall) {
        return out << "{ point: " << wall.location << ", size: " << wall.size << " }";
    }
}