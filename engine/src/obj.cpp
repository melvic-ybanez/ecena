//
// Created by Melvic Ybanez on 2/5/23.
//

#include <optional>
#include <istream>

#include "../include/obj.h"
#include "../../shared/include/utils.h"

namespace rt::obj {
    static std::optional<int> scan_int(const std::string& str);

    static std::optional<real> scan_real(const std::string& str);

    static std::vector<std::string> split(const std::string& str);

    Obj::Obj() {
        groups[default_group_name_] = std::make_unique<shapes::NamedGroup>(default_group_name_);
        current_group_ = groups[default_group_name_].get();
    }

    Obj Parser::parse(std::istream& is) {
        return Parser::parse_verbose(is).first;
    }

    const Point& Obj::vertex_at(size_t i) const {
        return vertices[i - 1];
    }

    const shapes::Triangle& Obj::triangle_at(size_t i, const std::string& group_name) const {
        return dynamic_cast<const shapes::Triangle&>(*group(group_name).children[i - 1]);
    }

    std::pair<Obj, int> Parser::parse_verbose(std::istream& is) {
        Parser parser;
        std::string line;
        int ignored_lines_count = 0;

        while (std::getline(is, line)) {
            if (auto vertex = Parser::parse_vertex(line); vertex.has_value()) {
                parser.obj.vertices.push_back(vertex.value());
            } else if (auto face = parser.parse_face(line); !face.empty()) {
                for (auto& child: face) {
                    if (parser.obj.current_group() == &parser.obj.default_group()) {
                        parser.obj.default_group().add_child(std::move(child));
                    } else {
                        parser.obj.current_group()->add_child(std::move(child));
                    }
                }
            } else if (!parser.parse_group(line)) ignored_lines_count++;
        }

        return {std::move(parser.obj), ignored_lines_count};
    }

    std::optional<Point> Parser::parse_vertex(const std::string& line) {
        if (!starts_with(line, "v ")) return std::nullopt;
        auto tokens = split(line.substr(2));
        if (tokens.size() != 3) return std::nullopt;

        std::vector<real> components;
        for (const auto& token: tokens) {
            auto comp = scan_real(token);
            if (!comp.has_value()) return std::nullopt;
            components.push_back(comp.value());
        }

        return {{components[0], components[1], components[2]}};
    }

    std::vector<std::unique_ptr<shapes::Triangle>> Parser::parse_face(const std::string& line) {
        if (!starts_with(line, "f ")) return {};
        auto tokens = split(line.substr(2));

        std::vector<Point> vertices;
        std::vector<std::unique_ptr<shapes::Triangle>> triangles;

        for (const auto& token: tokens) {
            auto vertex_index = scan_int(token);
            if (!vertex_index.has_value()) return {};
            auto value = vertex_index.value();

            // note that vertices in the OBJ data are 1-indexed
            if (value > 0 && value < obj.vertices.size() + 1) {
                vertices.push_back(obj.vertex_at(value));
            } else return {};
        }

        // perform fan triangulation
        for (int i = 1; i < vertices.size() - 1; i++) {
            auto tri = std::make_unique<shapes::Triangle>(vertices[0], vertices[i], vertices[i + 1]);
            triangles.push_back(std::move(tri));
        }

        return triangles;
    }

    bool Parser::parse_group(const std::string& line) {
        if (!starts_with(line, "g ")) return false;
        auto name = line.substr(2);
        obj.groups.insert({name, std::make_unique<shapes::NamedGroup>(name)});
        return obj.current_group(name);
    }

    shapes::Group& Obj::group(const std::string& name) const {
        return *groups[name];
    }

    shapes::Group& Obj::default_group() const {
        return group(default_group_name_);
    }

    shapes::Group* Obj::current_group() const {
        return current_group_;
    }

    bool Obj::current_group(const std::string& name) {
        if (groups.find(name) == groups.end()) return false;
        current_group_ = groups[name].get();
        return true;
    }

    std::unique_ptr<shapes::Group> Obj::to_group() const {
        auto group = std::make_unique<shapes::Group>();
        for (auto& [key, value]: groups) {
            group->add_child(std::move(value));
        }
        return group;
    }

    Obj& Obj::set_materials(std::unordered_map<std::string, std::unique_ptr<Material>>& group_mats) {
        for (auto& [key, group]: groups) {
            if (group_mats.find(key) != group_mats.end()) {
                group->material = std::move(group_mats.at(key));
                for (auto &child: group->children) {
                    child->material->reset(*group->material);
                }
            }
        }
        return *this;
    }

    std::optional<real> scan_real(const std::string& str) {
        size_t pos;
        auto value = std::stod(str, &pos);
        if (pos != str.size()) return std::nullopt;
        return value;
    }

    std::optional<int> scan_int(const std::string& str) {
        auto real = scan_real(str);
        if (!real.has_value() || (static_cast<int>(real.value()) == real.value()))
            return real;
        return std::nullopt;
    }

    std::vector<std::string> split(const std::string& str) {
        std::stringstream ss{str};
        std::vector<std::string> strs;
        std::string input;

        while (std::getline(ss, input, ' ')) {
            strs.push_back(input);
        }

        return strs;
    }
}