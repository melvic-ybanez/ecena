//
// Created by Melvic Ybanez on 12/26/22.
//

#include "../include/data.h"
#include "../../shared/include/utils.h"

namespace rt {
    Data::Data() : camera{100, 100, math::pi / 3} {}

    std::ostream& operator<<(std::ostream& out, const Data& data) {
        return out << "{ camera: " << data.camera << ", world: " << data.world << " }";
    }
}