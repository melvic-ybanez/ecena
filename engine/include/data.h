//
// Created by Melvic Ybanez on 12/26/22.
//

#ifndef ECENA_DATA_H
#define ECENA_DATA_H

#include <vector>
#include "../math/include/tuples.h"
#include "shapes.h"
#include "world.h"
#include "camera.h"

namespace rt {
    struct Data {
        World world;
        Camera camera;

        Data();
    };

    std::ostream& operator<<(std::ostream& out, const Data& data);
}

#endif //ECENA_DATA_H
