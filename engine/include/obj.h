//
// Created by Melvic Ybanez on 2/5/23.
//

#ifndef ECENA_OBJ_H
#define ECENA_OBJ_H

#include <string>
#include "../math/include/tuples.h"
#include "shapes.h"

namespace rt::obj {
    class Obj {
    public:
        std::unique_ptr<shapes::Group> group;

        Obj();

        [[nodiscard]] const Point &vertex_at(size_t i) const;

        [[nodiscard]] const shapes::Triangle &triangle_at(size_t i) const;

    private:
        friend std::pair<Obj, int> parse_verbose(std::istream &is);
        friend class Parser;

        std::vector<Point> vertices;
    };

    class Parser {
    public:
        std::optional<Point> parse_vertex(const std::string &line);

        std::optional<std::unique_ptr<shapes::Triangle>> parse_face(const std::string &line);

    private:
        friend std::pair<Obj, int> parse_verbose(std::istream &is);

        Obj obj;
    };

    Obj parse(std::istream &is);

    /**
     * Like `parse`, but also returns the number of ignored lines,
     * mainly for debugging or testing purposes.
     */
    std::pair<Obj, int> parse_verbose(std::istream &is);
}

#endif //ECENA_OBJ_H