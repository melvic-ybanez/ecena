//
// Created by Melvic Ybanez on 2/5/23.
//

#ifndef ECENA_OBJ_H
#define ECENA_OBJ_H

#include <string>
#include "../math/include/tuples.h"

namespace rt::obj {
    struct Obj {
        std::vector<Point> vertices;

        Point operator[](size_t i) const;
    };

    Obj parse(std::istream &is);

    /**
     * Like `parse`, but also returns the number of ignored lines,
     * mainly for debugging or testing purposes.
     */
    std::pair<Obj, int> parse_verbose(std::istream &is);
}

#endif //ECENA_OBJ_H