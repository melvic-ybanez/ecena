//
// Created by Melvic Ybanez on 2/5/23.
//

#include <optional>
#include <istream>

#include "../include/obj.h"
#include "../math/include/tuples.h"
#include "../include/shapes.h"
#include "../../shared/include/utils.h"

namespace rt::obj {
    static std::optional<int> scan_int(const std::string &str);

    static std::optional<real> scan_real(const std::string &str);

    static std::vector<std::string> split(const std::string &str);

    Obj::Obj() : group{std::make_unique<shapes::Group>()} {}

    Obj parse(std::istream &is) {
        return parse_verbose(is).first;
    }

    const Point &Obj::vertex_at(size_t i) const {
        return vertices[i - 1];
    }

    const shapes::Triangle &Obj::triangle_at(size_t i) const {
        return dynamic_cast<const shapes::Triangle &>(*group->children[i - 1]);
    }

    std::pair<Obj, int> parse_verbose(std::istream &is) {
        Parser parser;
        std::string line;
        int ignored_lines_count = 0;

        while (std::getline(is, line)) {
            if (auto vertex = parser.parse_vertex(line); vertex.has_value()) {
                parser.obj.vertices.push_back(vertex.value());
            } else if (auto face = parser.parse_face(line); face.has_value()) {
                parser.obj.group->add_child(std::move(face.value()));
            } else ignored_lines_count++;
        }

        return {std::move(parser.obj), ignored_lines_count};
    }

    std::optional<Point> Parser::parse_vertex(const std::string &line) {
        if (!starts_with(line, "v ")) return std::nullopt;
        auto tokens = split(line.substr(2));
        if (tokens.size() != 3) return std::nullopt;

        std::vector<real> components;
        for (const auto &token: tokens) {
            auto comp = scan_real(token);
            if (!comp.has_value()) return std::nullopt;
            components.push_back(comp.value());
        }

        return {{components[0], components[1], components[2]}};
    }

    std::optional<std::unique_ptr<shapes::Triangle>> Parser::parse_face(const std::string &line) {
        if (!starts_with(line, "f ")) return std::nullopt;
        auto tokens = split(line.substr(2));

        std::vector<Point> vertices;
        for (const auto &token: tokens) {
            auto vertex_index = scan_int(token);
            if (!vertex_index.has_value()) return std::nullopt;
            auto value = vertex_index.value();

            // note that vertices in the OBJ data are 1-indexed
            if (value > 0 && value < obj.vertices.size() + 1) {
                vertices.push_back(obj.vertex_at(value));
            } else return std::nullopt;
        }

        return std::make_unique<shapes::Triangle>(vertices[0], vertices[1], vertices[2]);
    }

    std::optional<real> scan_real(const std::string &str) {
        size_t pos;
        auto value = std::stod(str, &pos);
        if (pos != str.size()) return std::nullopt;
        return value;
    }

    std::optional<int> scan_int(const std::string &str) {
        auto real = scan_real(str);
        if (!real.has_value() || (static_cast<int>(real.value()) == real.value()))
            return real;
        return std::nullopt;
    }

    std::vector<std::string> split(const std::string &str) {
        std::stringstream ss{str};
        std::vector<std::string> strs;
        std::string input;

        while (std::getline(ss, input, ' ')) {
            strs.push_back(input);
        }

        return strs;
    }
}