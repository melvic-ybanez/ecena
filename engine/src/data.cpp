//
// Created by Melvic Ybanez on 12/26/22.
//

#include "../include/data.h"

namespace rt {
    math::Dimensions Data::pixel_size() const {
        return {wall.dimensions.width / canvas.width(), wall.dimensions.height / canvas.height()};
    }
}