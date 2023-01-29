//
// Created by Melvic Ybanez on 12/26/22.
//

#ifndef ECENA_RENDERER_H
#define ECENA_RENDERER_H

#include "ppm.h"
#include "data.h"

namespace rt {
    Ppm render(Data &data);

    void render_with_logging(Data &data, std::ofstream &out_img);
}

#endif //ECENA_RENDERER_H
