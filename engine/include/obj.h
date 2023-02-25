//
// Created by Melvic Ybanez on 2/5/23.
//

#ifndef ECENA_OBJ_H
#define ECENA_OBJ_H

#include <unordered_map>
#include "../math/include/tuples.h"
#include "shapes.h"

namespace rt::obj {
    class Obj {
    public:
        Obj();

        [[nodiscard]] const Point& vertex_at(size_t i) const;

        [[nodiscard]] const shapes::Triangle&
        triangle_at(size_t i, const std::string& group_name = default_group_name_) const;

        shapes::Group& group(const std::string& name) const;

        shapes::Group& default_group() const;

        bool current_group(const std::string& name);

        shapes::Group* current_group() const;

        std::unique_ptr<shapes::Group> to_group() const;

        constexpr static auto default_group_name_ = "default";

        Obj& set_materials(std::unordered_map<std::string, std::unique_ptr<Material>>& group_mats);

    private:
        friend class Parser;

        std::vector<Point> vertices;
        mutable std::unordered_map<std::string, std::unique_ptr<shapes::NamedGroup>> groups;

        shapes::Group* current_group_;
    };

    class Parser {
    public:
        static std::optional<Point> parse_vertex(const std::string& line);

        static Obj parse(std::istream& is);

        /**
         * Like `parse`, but also returns the number of ignored lines,
         * mainly for debugging or testing purposes.
         */
        static std::pair<Obj, int> parse_verbose(std::istream& is);

        std::vector<std::unique_ptr<shapes::Triangle>> parse_face(const std::string& line);

        bool parse_group(const std::string& line);

    private:
        Obj obj;
    };
}

#endif //ECENA_OBJ_H