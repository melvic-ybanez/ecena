//
// Created by Melvic Ybanez on 2/5/23.
//

#include <optional>
#include <istream>

#include "../include/obj.h"
#include "../../shared/include/utils.h"

namespace rt::obj {
    static std::optional<Point> parse_point(const std::string& line, const std::string& prefix);

    static std::optional<int> scan_int(const std::string& str);

    static std::optional<real> scan_real(const std::string& str);

    static std::vector<std::string> split(const std::string& str, char c = ' ');

    static int check_prefix(const std::string& line, const std::string& prefix);

    Obj::Obj() {
        groups[default_group_name_] = std::make_unique<shapes::NamedGroup>(default_group_name_);
        current_group_ = groups[default_group_name_].get();
    }

    Parser::Parser(GroupMats group_mats) : group_mats{std::move(group_mats)} {
        if (this->group_mats.find(Obj::default_group_name_) != this->group_mats.end()) {
            obj.default_group().material = this->group_mats.at(Obj::default_group_name_);
        }
    }

    Obj Parser::parse(std::istream& is) {
        return Parser::parse_verbose(is).first;
    }

    const Point& Obj::vertex_at(size_t i) const {
        return vertices[i - 1];
    }

    const Vec& Obj::normal_at(size_t i) const {
        return normals[i - 1];
    }

    const shapes::Triangle& Obj::triangle_at(size_t i, const std::string& group_name) const {
        return dynamic_cast<const shapes::Triangle&>(*group(group_name).children[i - 1]);
    }

    std::pair<Obj, int> Parser::parse_verbose(std::istream& is) {
        std::string line;
        int ignored_lines_count = 0;

        while (std::getline(is, line)) {
            replace_all(line, "\r", "");
            if (auto vertex = Parser::parse_vertex(line); vertex.has_value()) {
                obj.vertices.emplace_back(vertex.value());
            } else if (auto normal = Parser::parse_normal(line); normal.has_value()) {
                obj.normals.emplace_back(normal.value());
            } else if (auto face = parse_face(line); !face.empty()) {
                for (auto& child: face) {
                    if (obj.current_group() == &obj.default_group()) {
                        obj.default_group().add_child(std::move(child));
                    } else {
                        obj.current_group()->add_child(std::move(child));
                    }
                }
            } else if (!parse_group(line)) ignored_lines_count++;
        }

        return {std::move(obj), ignored_lines_count};
    }

    std::optional<Point> parse_point(const std::string& line, const std::string& prefix) {
        auto space_count = check_prefix(line, prefix);
        if (!space_count) return std::nullopt;
        auto tokens = split(line.substr(prefix.size() + space_count));
        if (tokens.size() != 3) return std::nullopt;

        std::vector<real> components;
        for (const auto& token: tokens) {
            auto comp = scan_real(token);
            if (!comp.has_value()) return std::nullopt;
            components.emplace_back(comp.value());
        }

        return {{components[0], components[1], components[2]}};
    }

    std::optional<Point> Parser::parse_vertex(const std::string& line) {
        return parse_point(line, "v");
    }

    std::optional<Point> Parser::parse_normal(const std::string& line) {
        return parse_point(line, "vn");
    }

    std::vector<std::unique_ptr<shapes::Triangle>> Parser::parse_face(const std::string& line) {
        auto space_count = check_prefix(line, "f");
        if (!space_count) return {};
        auto tokens = split(line.substr(space_count + 1));

        if (auto face = parse_face1(tokens); !face.empty()) return face;

        auto face = parse_face2(tokens);
        return face;
    }

    std::vector<std::unique_ptr<shapes::Triangle>> Parser::parse_face1(const std::vector<std::string>& tokens) {
        std::vector<int> vertex_indices;
        std::vector<std::unique_ptr<shapes::Triangle>> triangles;

        for (const auto& token: tokens) {
            auto maybe_vertex_index = scan_int(token);
            if (!maybe_vertex_index.has_value()) return {};
            auto vertex_index = maybe_vertex_index.value();

            // note that vertices in the OBJ data are 1-indexed
            if (vertex_index > 0 && vertex_index < obj.vertices.size() + 1) {
                vertex_indices.emplace_back(vertex_index);
            } else return {};
        }

        // perform fan triangulation
        for (int i = 1; i < vertex_indices.size() - 1; i++) {
            auto tri = std::make_unique<shapes::Triangle>(
                    obj.vertex_at(vertex_indices[0]),
                    obj.vertex_at(vertex_indices[i]),
                    obj.vertex_at(vertex_indices[i + 1]));
            tri->material = obj.current_group()->material;
            triangles.emplace_back(std::move(tri));
        }

        return triangles;
    }

    std::vector<std::unique_ptr<shapes::Triangle>> Parser::parse_face2(const std::vector<std::string>& tokens) {
        std::vector<int> vertex_indices;
        std::vector<int> normal_indices;
        std::vector<std::unique_ptr<shapes::Triangle>> triangles;

        for (const auto& token: tokens) {
            auto parts = split(token, '/');

            if (parts.size() != 3) return {};

            auto maybe_vertex_index = scan_int(parts[0]);
            if (!maybe_vertex_index.has_value()) return {};
            auto vertex_index = maybe_vertex_index.value();

            if (!parts[1].empty()) {
                auto maybe_texture = scan_int(parts[1]);
                if (!maybe_texture.has_value()) return {};
            }

            auto maybe_normal_vertex = scan_int(parts[2]);
            if (!maybe_normal_vertex.has_value()) return {};
            auto normal_index = maybe_normal_vertex.value();

            // note that vertices in the OBJ data are 1-indexed
            if (vertex_index > 0 && vertex_index <= obj.vertices.size()) {
                vertex_indices.emplace_back(vertex_index);
            } else return {};

            // normal vertices are also 1-indexed
            if (normal_index > 0 && normal_index <= obj.normals.size()) {
                normal_indices.emplace_back(normal_index);
            } else return {};
        }

        // perform fan triangulation
        for (int i = 1; i < vertex_indices.size() - 1; i++) {
            auto tri = std::make_unique<shapes::SmoothTriangle>(
                    obj.vertex_at(vertex_indices[0]),
                    obj.vertex_at(vertex_indices[i]),
                    obj.vertex_at(vertex_indices[i + 1]),
                    obj.normal_at(normal_indices[0]),
                    obj.normal_at(normal_indices[i]),
                    obj.normal_at(normal_indices[i + 1]));
            tri->material = obj.current_group()->material;
            triangles.emplace_back(std::move(tri));
        }

        return triangles;
    }

    bool Parser::parse_group(const std::string& line) {
        auto space_count = check_prefix(line, "g");
        if (!space_count) return false;
        auto name = line.substr(space_count + 1);
        auto group = std::make_unique<shapes::NamedGroup>(name);

        if (group_mats.find(name) != group_mats.end()) {
            group->material = group_mats.at(name);
        }

        obj.groups.insert({name, std::move(group)});

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

    std::vector<std::string> split(const std::string& str, char sep) {
        std::stringstream ss{str};
        std::vector<std::string> strs;
        std::string input;

        while (std::getline(ss, input, sep)) {
            strs.emplace_back(input);
        }

        return strs;
    }

    Obj parse(std::istream& is) {
        return Parser{}.parse(is);
    }

    std::pair<Obj, int> parse_verbose(std::istream& is) {
        return Parser{}.parse_verbose(is);
    }

    int check_prefix(const std::string& line, const std::string& prefix) {
        if (!starts_with(line, prefix)) return 0;
        int space_count = 0;
        auto rest = line.substr(prefix.size());

        while (rest[0] == ' ') {
            space_count++;
            rest = rest.substr(1);
        }
        return space_count;
    }
}