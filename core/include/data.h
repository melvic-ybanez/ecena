//
// Created by Melvic Ybanez on 12/26/22.
//

#ifndef ECENA_DATA_H
#define ECENA_DATA_H

#include <vector>
#include "../../math/include/tuples.h"
#include "canvas.h"
#include "shapes.h"

namespace rt {
    struct Wall {
        Point location;
        math::Dimensions dimensions;
    };

    class Data {
    public:
        Point ray_origin;
        Wall wall;
        Canvas canvas;
        std::vector<std::unique_ptr<Shape>> shapes;

        math::Dimensions pixel_size() const;
    };
}

#endif //ECENA_DATA_H
