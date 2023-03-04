//
// Created by Melvic Ybanez on 2/5/23.
//

#ifndef ECENA_OBJ_H
#define ECENA_OBJ_H

#include <unordered_map>
#include "../math/include/tuples.h"
#include "shapes.h"

namespace rt::obj {
    using GroupMats = std::unordered_map<std::string, Material*>;

    class Obj {
    public:
        Obj();

        [[nodiscard]] const Point& vertex_at(size_t i) const;

        [[nodiscard]] const Vec& normal_at(size_t t) const;

        [[nodiscard]] const shapes::Triangle&
        triangle_at(size_t i, const std::string& group_name = default_group_name_) const;

        shapes::Group& group(const std::string& name) const;

        shapes::Group& default_group() const;

        bool current_group(const std::string& name);

        shapes::Group* current_group() const;

        std::unique_ptr<shapes::Group> to_group() const;

        constexpr static auto default_group_name_ = "default";

    private:
        friend class Parser;

        std::vector<Point> vertices;
        std::vector<Vec> normals;
        mutable std::unordered_map<std::string, std::unique_ptr<shapes::NamedGroup>> groups;

        shapes::Group* current_group_;
    };

    class Parser {
    public:
        Parser(GroupMats group_mats = {});

        Obj parse(std::istream& is);

        /**
         * Like `parse`, but also returns the number of ignored lines,
         * mainly for debugging or testing purposes.
         */
        std::pair<Obj, int> parse_verbose(std::istream& is);

        std::vector<std::unique_ptr<shapes::Triangle>> parse_face(const std::string& line);

        bool parse_group(const std::string& line);

        static std::optional<Point> parse_normal(const std::string& line);

        static std::optional<Point> parse_vertex(const std::string& line);

    private:
        Obj obj;
        GroupMats group_mats;

        /**
         * Parses the first variant of the face command.
         * Format: `f <index1> <index2> <index3>`
         * Example: `f 1 2 3`
         */
        std::vector<std::unique_ptr<shapes::Triangle>> parse_face1(const std::vector<std::string>& tokens);

        /**
         * Parses the second variant of the face command.
         * Format: `f <component1> <component2> <component3>`, where each `component` is of the
         * form `<index>/[texture]/<normal_index>`.
         * Examples:
         *     `f 1/2/3 2/3/4 2/4/5`
         *     `f 1//3 2//4 3//5`
         */
        std::vector<std::unique_ptr<shapes::Triangle>> parse_face2(const std::vector<std::string>& tokens);
    };

    Obj parse(std::istream& is);

    std::pair<Obj, int> parse_verbose(std::istream& is);
}

#endif //ECENA_OBJ_H