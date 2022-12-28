//
// Created by Melvic Ybanez on 12/26/22.
//

#include "../include/data.h"
#include "../../shared/include/utils.h"

namespace rt {
    Data::Data() {
        wall.dimensions = {100, 100};
    }


    math::Dimensions Data::pixel_size() const {
        return {wall.dimensions.width / canvas.width(), wall.dimensions.height / canvas.height()};
    }

    std::ostream &operator<<(std::ostream &out, const Data &data) {
        return out << "{ ray_origin: " << data.ray_origin
                   << ", wall: " << data.wall << ", canvas: " << data.canvas
                   << ", pixel_size: " << data.pixel_size() << ", shapes: "
                   << join_array(data.shapes) << " }";
    }

    std::ostream &operator<<(std::ostream &out, const Wall &wall) {
        return out << "{ point: " << wall.location << ", dimensions: " << wall.dimensions << " }";
    }
}