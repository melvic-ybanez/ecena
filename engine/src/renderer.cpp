//
// Created by Melvic Ybanez on 12/26/22.
//

#include "../include/renderer.h"

namespace rt {
    Ppm render(Data &data) {
        return Ppm{data.camera.render(data.world)};
    }
}