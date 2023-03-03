//
// Created by Melvic Ybanez on 12/27/22.
//

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "../include/eval.h"
#include "../include/errors.h"
#include "../../engine/include/obj.h"

#define SKIP_DOC_FIELDS_OF(field) if (field.key() == "name" || field.key() == "description") continue
#define SKIP_DOC_FIELDS SKIP_DOC_FIELDS_OF(field)
#define IGNORE_ERRORS(fn) try { fn; } catch (...) {}

namespace rt::dsl::eval {
    static Point to_point(const Expr& expr, int line);

    static Vec to_vec(const Expr& expr, int line);

    static Color to_color(const Expr& expr, int line);

    static const Array* to_array(const Expr& expr, std::optional<size_t> size, int line);

    template<typename T, typename Fn>
    static std::vector<T> to_array_of(const Expr& expr, std::optional<size_t> size, int line, Fn fn);

    static std::vector<double> to_num_array(const Expr& expr, size_t size, int line);

    static const Object* to_object(const Expr& expr, int line);

    static const Number* to_num(const Expr& expr, int line);

    static const String* to_str(const Expr& expr, int line);

    static const Boolean* to_bool(const Expr& expr, int line);

    static real to_real(const Expr& expr, int line);

    static Matrix<4, 4> to_transform(const Expr& expr, int line);

    static std::unique_ptr<shapes::CylinderLike>
    to_cylinder_like(shapes::CylinderLike* cylinder_like, const Object* obj);

    static std::unique_ptr<shapes::Group>
    to_group(World& world, const Object* obj, std::unique_ptr<shapes::Group> group = std::make_unique<shapes::Group>());

    static obj::Obj to_obj(World& world, const Object* obj, int line);

    static std::unique_ptr<shapes::Triangle> to_triangle(const Object* obj);

    static void throw_unknown_field_error(const Field& field);

    static void abort(const errors::Error& error);

    template<typename>
    static void make_pattern(std::unique_ptr<Pattern>& pattern, const std::vector<Color>& components);

    template<typename T, ExprType>
    static const T* to_expr(const Expr& expr, int line);

    Data to_data(const Object& object) {
        Data data;

        for (const auto& field: object.fields) {
            if (field.key() == "camera") data.camera = to_camera(field.value(), field.line);
            else if (field.key() == "world") data.world = to_world(field.value(), field.line);
            else throw_unknown_field_error(field);
        }

        return data;
    }

    Camera to_camera(const Expr& expr, int line) {
        auto obj = to_object(expr, line);
        Camera camera;

        for (const auto& field: obj->fields) {
            auto& key = field.key();
            auto& value = field.value();

            if (key == "h_size") camera.h_size = to_real(value, field.line);
            else if (key == "v_size") camera.v_size = to_real(value, field.line);
            else if (key == "field_of_view") camera.field_of_view = to_real(value, field.line);
            else if (key == "transform") {
                auto arr = to_array(value, 3, field.line);
                auto from = to_point(*arr->elems[0], field.line);
                auto to = to_point(*arr->elems[1], field.line);
                auto up = to_vec(*arr->elems[2], field.line);
                camera.transform = math::matrix::view_transform(from, to, up);
            } else if (key == "anti-aliasing") camera.antialias = to_bool(value, field.line)->value;
            else if (key == "background") {
                try {
                    camera.bg_colors = to_color(value, field.line);
                } catch (const errors::Error& error) {
                    auto colors = to_array_of<Color>(value, 2, field.line, to_color);
                    camera.bg_colors = std::make_pair(colors[0], colors[1]);
                }
            } else throw_unknown_field_error(field);
        }

        return camera;
    }

    World to_world(const Expr& expr, int line) {
        auto obj = to_object(expr, line);
        World world;

        for (const auto& field: obj->fields) {
            if (field.key() == "objects") world.objects = to_shapes(world, field.value(), field.line);
            else if (field.key() == "light") world.light = to_point_light(field.value(), field.line);
            else throw_unknown_field_error(field);
        }

        return world;
    }

    std::vector<std::unique_ptr<Shape>> to_shapes(World& world, const Expr& expr, int line) {
        auto arr = to_array(expr, std::nullopt, line);
        std::vector<std::unique_ptr<Shape>> shapes;

        for (const auto& elem: arr->elems) {
            if (auto shape = to_shape(world, *elem, line); shape != nullptr) {
                shapes.emplace_back(std::move(shape));
            }
        }

        return shapes;
    }

    std::unique_ptr<Shape> to_shape(World& world, const Expr& expr, int line) {
        auto obj = to_object(expr, line);
        std::unique_ptr<Shape> shape;

        bool has_type = false;
        for (const auto& field: obj->fields) {
            if (field.key() == "type") {
                auto type = to_str(field.value(), field.line);
                if (*type == "sphere") shape = std::make_unique<shapes::Sphere>();
                if (*type == "plane") shape = std::make_unique<shapes::Plane>();
                if (*type == "cube") shape = std::make_unique<shapes::Cube>();
                if (*type == "cylinder") shape = to_cylinder_like(new shapes::Cylinder, obj);
                if (*type == "cone") shape = to_cylinder_like(new shapes::Cone, obj);
                if (*type == "group") shape = to_group(world, obj);
                if (*type == "obj") shape = to_group(world, obj, to_obj(world, obj, field.line).to_group());
                if (*type == "triangle") shape = to_triangle(obj);

                has_type = true;
            }
        }
        if (!has_type) abort(errors::required_field("type", line));

        std::array<std::string, 12> parsed_keys{
                "type", "minimum", "maximum", "closed", "children", "threshold", "points", "min",
                "max", "path", "source", "materials"
        };

        for (const auto& field: obj->fields) {
            SKIP_DOC_FIELDS;
            auto& key = field.key();
            if (std::find_if(parsed_keys.begin(), parsed_keys.end(), [&](auto k) { return key == k; }) !=
                parsed_keys.end()) {
                // we are skipping the parsed fields, so we don't accidentally mistake them for unknown fields
                continue;
            }
            if (key == "material") {
                shape->material = to_material(world, shape->type(), field.value(), field.line);
            } else if (key == "transform") {
                shape->transformation = to_transform(field.value(), field.line);
            } else throw_unknown_field_error(field);
        }

        return shape;
    }

    std::unique_ptr<shapes::CylinderLike> to_cylinder_like(shapes::CylinderLike* cylinder_like, const Object* obj) {
        for (const auto& field: obj->fields) {
            auto& key = field.key();
            if (key == "minimum" || key == "min") cylinder_like->min = to_real(field.value(), field.line);
            if (key == "maximum" || key == "max") cylinder_like->max = to_real(field.value(), field.line);
            if (key == "closed") cylinder_like->closed = to_bool(field.value(), field.line);
        }
        return std::unique_ptr<shapes::CylinderLike>(cylinder_like);
    }

    std::unique_ptr<shapes::Group> to_group(World& world, const Object* obj, std::unique_ptr<shapes::Group> group) {
        for (const auto& field: obj->fields) {
            if (field.key() == "children") {
                auto children = to_shapes(world, field.value(), field.line);
                for (auto& child: children) {
                    group->add_child(std::move(child));
                }
            }
        }
        for (const auto& field: obj->fields) {
            if (field.key() == "threshold") {
                auto threshold = to_real(field.value(), field.line);
                group->divide(static_cast<int>(threshold));
            }
        }
        return group;
    }

    std::unique_ptr<shapes::Triangle> to_triangle(const Object* obj) {
        for (const auto& field: obj->fields) {
            if (field.key() == "points") {
                auto components = to_array_of<Point>(field.value(), 3, field.line, to_point);
                return std::make_unique<shapes::Triangle>(components[0], components[1], components[2]);
            }
        }
        return nullptr;
    }

    obj::Obj to_obj(World& world, const Object* object, int line) {
        obj::GroupMats group_mats;

        for (const auto& field: object->fields) {
            if (field.key() == "materials") {
                auto arr = to_array(field.value(), std::nullopt, field.line);

                for (const auto& elem: arr->elems) {
                    auto kv = to_array(*elem, 2, field.line);
                    auto group_name = to_str(*kv->elems[0], field.line)->value;
                    auto mat = to_material(world, shapes::Type::group, *kv->elems[1], field.line);
                    group_mats.insert(std::make_pair<std::string, Material*>(
                            std::move(group_name), std::move(mat)));
                }
            }
        }

        obj::Parser parser{group_mats};

        for (const auto& field: object->fields) {
            if (field.key() == "path") {
                auto path = to_str(field.value(), field.line)->value;
                std::ifstream in{path};
                if (!in) throw errors::invalid_path(path, field.line);
                return std::move(parser.parse(in));
            } else if (field.key() == "source") {
                auto source = to_str(field.value(), field.line);
                std::stringstream in{source->value};
                return std::move(parser.parse(in));
            }
        }
        throw errors::obj_not_found(line);
    }

    Material* to_material(World& world, shapes::Type shape_type, const Expr& expr, int line) {
        IGNORE_ERRORS(
                auto mat_str = to_str(expr, line);
                if (*mat_str == "glass") {
                    auto material = new Material{materials::glass()};
                    world.materials.emplace_back(std::unique_ptr<Material>(material));
                    return material;
                };
        )

        auto obj = to_object(expr, line);
        auto material = new Material;

        for (auto& field: obj->fields) {
            SKIP_DOC_FIELDS;
            auto& key = field.key();
            auto& value = field.value();

            if (key == "color") material->color = to_color(value, field.line);
            else if (key == "specular") material->specular = to_real(value, field.line);
            else if (key == "diffuse") material->diffuse = to_real(value, field.line);
            else if (key == "pattern") material->pattern = to_pattern(shape_type, value, field.line);
            else if (key == "reflectivity" || key == "reflective") material->reflectivity = to_real(value, field.line);
            else if (key == "ambient") material->ambient = to_real(value, field.line);
            else if (key == "shininess") material->shininess = to_real(value, field.line);
            else if (key == "transparency") material->transparency = to_real(value, field.line);
            else if (key == "refractive_index") material->refractive_index = to_real(value, field.line);
            else throw_unknown_field_error(field);
        }

        world.materials.emplace_back(std::unique_ptr<Material>(material));

        return material;
    }

    std::unique_ptr<Pattern> to_pattern(shapes::Type shape_type, const Expr& expr, int line) {
        if (expr.type() == ExprType::null) return nullptr;

        auto obj = to_object(expr, line);
        std::unique_ptr<Pattern> pattern;

        std::vector<Color> components;
        std::string uv_pattern_str;
        std::string type;
        std::string path;
        int field_line;

        for (auto& field: obj->fields) {
            auto& key = field.key();
            field_line = field.line;
            if (key == "type") {
                type = to_str(field.value(), field_line)->value;
            } else if (key == "components") {
                components = to_array_of<Color>(field.value(), std::nullopt, field_line, to_color);
            } else if (key == "uv_pattern") {
                uv_pattern_str = to_str(field.value(), field_line)->value;
            } else if (key == "path") {
                path = to_str(field.value(), field_line)->value;
            }
        }

        if (type.empty()) abort(errors::required_field("type", line));

        if (type == "stripe") {
            make_pattern<patterns::Stripe>(pattern, components);
        } else if (type == "gradient") {
            make_pattern<patterns::Gradient>(pattern, components);
        } else if (type == "checkers") {
            make_pattern<patterns::Checkers>(pattern, components);
        } else if (type == "ring") {
            make_pattern<patterns::Ring>(pattern, components);
        } else if (type == "texture") {
            if (uv_pattern_str.empty()) abort(errors::required_field("uv_pattern", line));

            uv::Map uv_map;
            std::unique_ptr<UV> uv_pattern;

            if (uv_pattern_str == "image") {
                if (path.empty()) abort(errors::required_field("path", line));

                std::ifstream in{path};
                if (!in) abort(errors::invalid_path(path, field_line));
                auto canvas = canvas::from_ppm(in);
                if (!canvas.has_value()) abort(errors::unable_to_load_path(path, field_line));

                uv_pattern = std::make_unique<uv::Image>(canvas.value());
            } else throw errors::invalid_kind(uv_pattern_str, "UV Pattern", field_line);

            if (shape_type == shapes::Type::sphere) {
                uv_map = uv::maps::spherical;
            }

            pattern = std::make_unique<patterns::TextureMap>(std::move(uv_pattern), uv_map);
        } else throw errors::invalid_kind(type, "Pattern", field_line);

        for (auto& field: obj->fields) {
            SKIP_DOC_FIELDS;
            auto& key = field.key();
            if (key == "type" || key == "components" || key == "path" || key == "uv_pattern") continue;
            if (key == "transform") pattern->transformation = to_transform(field.value(), field.line);
            else throw_unknown_field_error(field);
        }

        return pattern;
    }

    template<typename T>
    void make_pattern(std::unique_ptr<Pattern>& pattern, const std::vector<Color>& components) {
        pattern = std::make_unique<T>(components[0], components[1]);
    }

    Matrix<4, 4> to_transform(const Expr& expr, int line) {
        auto arr = to_array(expr, std::nullopt, line);
        std::vector<Matrix<4, 4>> transforms;

        for (auto& elem: arr->elems) {
            auto t = to_array(*elem, 2, line);
            auto func = to_str(*t->elems[0], line);
            if (*func == "scale") {
                auto values = to_num_array(*t->elems[1], 3, line);
                transforms.emplace_back(matrix::scaling(values[0], values[1], values[2]));
            } else if (*func == "translate") {
                auto values = to_num_array(*t->elems[1], 3, line);
                transforms.emplace_back(matrix::translation(values[0], values[1], values[2]));
            } else if (*func == "rotate_x") {
                auto r = to_real(*t->elems[1], line);
                transforms.emplace_back(matrix::rotation_x(r));
            } else if (*func == "rotate_y") {
                auto r = to_real(*t->elems[1], line);
                transforms.emplace_back(matrix::rotation_y(r));
            } else if (*func == "rotate_z") {
                auto r = to_real(*t->elems[1], line);
                transforms.emplace_back(matrix::rotation_z(r));
            } else if (*func == "shear") {
                auto values = to_num_array(*t->elems[1], 6, line);
                transforms.emplace_back(
                        matrix::shearing(values[0], values[1], values[2], values[3], values[4], values[5]));
            }
        }

        auto transform = matrix::identity<4, 4>();
        std::reverse(transforms.begin(), transforms.end());
        for (const auto& t: transforms) {
            transform = transform * t;
        }

        return transform;
    }

    const Object* to_object(const Expr& expr, int line) {
        if (expr.type() != ExprType::object) {
            throw errors::type_mismatch(ExprType::object, expr.type(), line);
        }

        return dynamic_cast<const Object*>(&expr);
    }

    Point to_point(const Expr& expr, int line) {
        auto elem_values = to_num_array(expr, 3, line);
        return {elem_values[0], elem_values[1], elem_values[2]};
    }

    Color to_color(const Expr& expr, int line) {
        IGNORE_ERRORS(
                auto color_str = to_str(expr, line);
                if (*color_str == "white") return Color::white_;
                if (*color_str == "black") return Color::black_;
        )

        auto point = to_point(expr, line);
        return Color{point.x(), point.y(), point.z()};
    }

    Vec to_vec(const Expr& expr, int line) {
        auto point = to_point(expr, line);
        return {point.x(), point.y(), point.z()};
    }

    const Array* to_array(const Expr& expr, std::optional<size_t> size, int line) {
        if (expr.type() != ExprType::array) {
            throw errors::type_mismatch(ExprType::array, expr.type(), line);
        }

        auto arr = dynamic_cast<const Array*>(&expr);

        if (size.has_value() && arr->elems.size() != size) {
            throw errors::wrong_args_count(size.value(), arr->elems.size(), line);
        }

        return arr;
    }

    template<typename T, typename Fn>
    std::vector<T> to_array_of(const Expr& expr, std::optional<size_t> size, int line, Fn fn) {
        auto arr = to_array(expr, size, line);
        std::vector<T> elem_values;

        for (const auto& elem: arr->elems) {
            elem_values.emplace_back(fn(*elem, line));
        }

        return elem_values;
    }

    template<typename T, ExprType E>
    const T* to_expr(const Expr& expr, int line) {
        if (expr.type() != E)
            throw errors::type_mismatch(E, expr.type(), line);
        return dynamic_cast<const T*>(&expr);
    }

    const Number* to_num(const Expr& expr, int line) {
        return to_expr<Number, ExprType::number>(expr, line);
    }

    const Boolean* to_bool(const Expr& expr, int line) {
        return to_expr<Boolean, ExprType::boolean>(expr, line);
    }

    real to_real(const Expr& expr, int line) {
        return to_num(expr, line)->value;
    }

    const String* to_str(const Expr& expr, int line) {
        return to_expr<String, ExprType::string>(expr, line);
    }

    std::vector<double> to_num_array(const Expr& expr, size_t size, int line) {
        return to_array_of<double>(expr, size, line, to_real);
    }

    PointLight to_point_light(const Expr& expr, int line) {
        auto object = to_object(expr, line);
        PointLight light;

        for (auto& field: object->fields) {
            SKIP_DOC_FIELDS;
            if (field.key() == "position") light.position = to_point(field.value(), field.line);
            else if (field.key() == "intensity") light.intensity = to_color(field.value(), field.line);
            else throw_unknown_field_error(field);
        }

        return light;
    }

    void throw_unknown_field_error(const Field& field) {
        abort(errors::unknown_field(field.key().value, field.line));
    }

    void abort(const errors::Error& error) {
        throw error;
    }
}