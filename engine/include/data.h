//
// Created by Melvic Ybanez on 12/26/22.
//

#ifndef ECENA_DATA_H
#define ECENA_DATA_H

#include <vector>
#include "../math/include/tuples.h"
#include "canvas.h"
#include "shapes.h"
#include "light.h"

namespace rt {
    struct Wall {
        Point location;
        math::Dimensions size;
    };

    class Data {
    public:
        Point ray_origin;
        Wall wall;
        Canvas canvas;
        std::vector<std::unique_ptr<Shape>> shapes;
        PointLight light;

        Data();

        math::Dimensions pixel_size() const;
    };

    std::ostream &operator<<(std::ostream &out, const Data &data);

    std::ostream &operator<<(std::ostream &out, const Wall &wall);
}

#endif //ECENA_DATA_H
