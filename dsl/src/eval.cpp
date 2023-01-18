//
// Created by Melvic Ybanez on 12/27/22.
//

#include "../include/eval.h"
#include "../include/errors.h"

#define SKIP_DOC_FIELDS_OF(field) if (field.key() == "name" || field.key() == "description") continue
#define SKIP_DOC_FIELDS SKIP_DOC_FIELDS_OF(field)
#define IGNORE_ERRORS(fn) try { fn; } catch (...) {}

namespace rt::dsl::eval {
    static Point to_point(const Expr &expr, int line);

    static Vec to_vec(const Expr &expr, int line);

    static Color to_color(const Expr &expr, int line);

    static const Array *to_array(const Expr &expr, std::optional<size_t> size, int line);

    template<typename T, typename Fn>
    static std::vector<T> to_array_of(const Expr &expr, std::optional<size_t> size, int line, Fn fn);

    static std::vector<double> to_num_array(const Expr &expr, size_t size, int line);

    static const Object *to_object(const Expr &expr, int line);

    static const Number *to_num(const Expr &expr, int line);

    static const String *to_str(const Expr &expr, int line);

    static const Boolean *to_bool(const Expr &expr, int line);

    static real to_real(const Expr &expr, int line);

    static Matrix<4, 4> to_transform(const Expr &expr, int line);

    static void init_cylinder_like(shapes::CylinderLike *cylinder_like, const Object *obj);

    static void throw_unknown_field_error(const Field &field);

    template<typename>
    static void make_pattern(std::unique_ptr<Pattern> &pattern, const std::vector<Color> &components);

    template<typename T, ExprType>
    static const T *to_expr(const Expr &expr, int line);

    Data to_data(const Object &object) {
        Data data;

        for (auto &field: object.fields) {
            if (field.key() == "camera") data.camera = to_camera(*field.value_, field.line);
            else if (field.key() == "world") data.world = to_world(*field.value_, field.line);
            else throw_unknown_field_error(field);
        }

        return data;
    }

    Camera to_camera(const Expr &expr, int line) {
        auto obj = to_object(expr, line);
        Camera camera;

        for (auto &field: obj->fields) {
            auto &key = field.key();
            auto &value = *field.value_;

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
                } catch (const errors::Error &error) {
                    auto colors = to_array_of<Color>(value, 2, field.line, to_color);
                    camera.bg_colors = std::make_pair(colors[0], colors[1]);
                }
            } else throw_unknown_field_error(field);
        }

        return camera;
    }

    World to_world(const Expr &expr, int line) {
        auto obj = to_object(expr, line);
        World world;

        for (auto &field: obj->fields) {
            if (field.key() == "objects") world.objects = to_shapes(*field.value_, field.line);
            else if (field.key() == "light") world.light = to_point_light(*field.value_, field.line);
            else throw_unknown_field_error(field);
        }

        return world;
    }

    std::vector<std::unique_ptr<Shape>> to_shapes(const Expr &expr, int line) {
        auto arr = to_array(expr, {}, line);
        std::vector<std::unique_ptr<Shape>> shapes;

        for (auto &elem: arr->elems) {
            if (auto shape = to_shape(*elem, line); shape != nullptr) {
                shapes.push_back(std::move(shape));
            }
        }

        return shapes;
    }

    std::unique_ptr<Shape> to_shape(const Expr &expr, int line) {
        auto obj = to_object(expr, line);
        std::unique_ptr<Shape> shape;

        bool has_type = false;
        for (auto &field: obj->fields) {
            if (field.key() == "type") {
                auto type = to_str(*field.value_, field.line);
                if (*type == "sphere") shape = std::make_unique<shapes::Sphere>();
                if (*type == "plane") shape = std::make_unique<shapes::Plane>();
                if (*type == "cube") shape = std::make_unique<shapes::Cube>();
                if (*type == "cylinder") {
                    auto cylinder = new shapes::Cylinder;
                    init_cylinder_like(cylinder, obj);
                    shape = std::unique_ptr<shapes::Cylinder>(cylinder);
                }
                if (*type == "cone") {
                    auto cone = new shapes::Cone;
                    init_cylinder_like(cone, obj);
                    shape = std::unique_ptr<shapes::Cone>(cone);
                }

                has_type = true;
            }
        }
        if (!has_type) throw errors::required_type("type", line);

        for (auto &field: obj->fields) {
            SKIP_DOC_FIELDS;
            auto &key = field.key();
            if (key == "type" || key == "minimum" || key == "maximum" || key == "closed") continue;
            if (key == "material") {
                shape->material = to_material(*field.value_, field.line);
            } else if (key == "transform") {
                shape->transformation = to_transform(*field.value_, field.line);
            } else throw_unknown_field_error(field);
        }

        return shape;
    }

    void init_cylinder_like(shapes::CylinderLike *cylinder_like, const Object *obj) {
        for (auto &field: obj->fields) {
            auto &key = field.key();
            if (key == "minimum") cylinder_like->minimum = to_real(*field.value_, field.line);
            if (key == "maximum") cylinder_like->maximum = to_real(*field.value_, field.line);
            if (key == "closed") cylinder_like->closed = to_bool(*field.value_, field.line);
        }
    }

    std::unique_ptr<Material> to_material(const Expr &expr, int line) {
        IGNORE_ERRORS(
                auto mat_str = to_str(expr, line);
                if (*mat_str == "glass") return std::unique_ptr<Material>{new Material{materials::glass()}};
        )

        auto obj = to_object(expr, line);
        auto material = std::make_unique<Material>();

        for (auto &field: obj->fields) {
            SKIP_DOC_FIELDS;
            auto &key = field.key();
            auto &value = *field.value_;

            if (key == "color") material->color = to_color(value, field.line);
            else if (key == "specular") material->specular = to_real(value, field.line);
            else if (key == "diffuse") material->diffuse = to_real(value, field.line);
            else if (key == "pattern") material->pattern = to_pattern(value, field.line);
            else if (key == "reflectivity") material->reflectivity = to_real(value, field.line);
            else if (key == "ambient") material->ambient = to_real(value, field.line);
            else if (key == "shininess") material->shininess = to_real(value, field.line);
            else if (key == "transparency") material->transparency = to_real(value, field.line);
            else if (key == "refractive_index") material->refractive_index = to_real(value, field.line);
            else throw_unknown_field_error(field);
        }

        return material;
    }

    std::unique_ptr<Pattern> to_pattern(const Expr &expr, int line) {
        if (expr.type() == ExprType::null) {
            return nullptr;
        }

        auto obj = to_object(expr, line);
        std::unique_ptr<Pattern> pattern;

        std::vector<Color> components;
        std::string type;
        int type_line;

        for (auto &field: obj->fields) {
            if (field.key() == "type") {
                type = to_str(*field.value_, field.line)->value;
                type_line = field.line;
            } else if (field.key() == "components") {
                components = to_array_of<Color>(*field.value_, {}, field.line, to_color);
            }
        }

        if (type.empty()) throw errors::required_type("type", line);

        if (type == "stripe") {
            make_pattern<patterns::Stripe>(pattern, components);
        } else if (type == "gradient") {
            make_pattern<patterns::Gradient>(pattern, components);
        } else if (type == "checkers") {
            make_pattern<patterns::Checkers>(pattern, components);
        } else if (type == "ring") {
            make_pattern<patterns::Ring>(pattern, components);
        } else throw errors::invalid_kind(type, "Pattern", type_line);

        for (auto &field: obj->fields) {
            SKIP_DOC_FIELDS;
            if (field.key() == "type" || field.key() == "components") continue;
            if (field.key() == "transform") pattern->transformation = to_transform(*field.value_, field.line);
            else throw_unknown_field_error(field);
        }

        return pattern;
    }

    template<typename T>
    void make_pattern(std::unique_ptr<Pattern> &pattern, const std::vector<Color> &components) {
        pattern = std::make_unique<T>(components[0], components[1]);
    }

    Matrix<4, 4> to_transform(const Expr &expr, int line) {
        auto arr = to_array(expr, {}, line);
        std::vector<Matrix<4, 4>> transforms;

        for (auto &elem: arr->elems) {
            auto t = to_array(*elem, 2, line);
            auto func = to_str(*t->elems[0], line);
            if (*func == "scale") {
                auto values = to_num_array(*t->elems[1], 3, line);
                transforms.push_back(matrix::scaling(values[0], values[1], values[2]));
            } else if (*func == "translate") {
                auto values = to_num_array(*t->elems[1], 3, line);
                transforms.push_back(matrix::translation(values[0], values[1], values[2]));
            } else if (*func == "rotate_x") {
                auto r = to_real(*t->elems[1], line);
                transforms.push_back(matrix::rotation_x(r));
            } else if (*func == "rotate_y") {
                auto r = to_real(*t->elems[1], line);
                transforms.push_back(matrix::rotation_y(r));
            } else if (*func == "rotate_z") {
                auto r = to_real(*t->elems[1], line);
                transforms.push_back(matrix::rotation_z(r));
            } else if (*func == "shearing") {
                auto values = to_num_array(*t->elems[1], 6, line);
                transforms.push_back(
                        matrix::shearing(values[0], values[1], values[2], values[3], values[4], values[5]));
            }
        }

        auto transform = matrix::identity<4, 4>();
        std::reverse(transforms.begin(), transforms.end());
        for (auto t: transforms) {
            transform = transform * t;
        }

        return transform;
    }

    const Object *to_object(const Expr &expr, int line) {
        if (expr.type() != ExprType::object) {
            throw errors::type_mismatch(ExprType::object, expr.type(), line);
        }

        return dynamic_cast<const Object *>(&expr);
    }

    Point to_point(const Expr &expr, int line) {
        auto elem_values = to_num_array(expr, 3, line);
        return {elem_values[0], elem_values[1], elem_values[2]};
    }

    Color to_color(const Expr &expr, int line) {
        IGNORE_ERRORS(
                auto color_str = to_str(expr, line);
                if (*color_str == "white") return Color::white_;
                if (*color_str == "black") return Color::black_;
        )

        auto point = to_point(expr, line);
        return Color{point.x(), point.y(), point.z()};
    }

    Vec to_vec(const Expr &expr, int line) {
        auto point = to_point(expr, line);
        return {point.x(), point.y(), point.z()};
    }

    const Array *to_array(const Expr &expr, std::optional<size_t> size, int line) {
        if (expr.type() != ExprType::array) {
            throw errors::type_mismatch(ExprType::array, expr.type(), line);
        }

        auto arr = dynamic_cast<const Array *>(&expr);

        if (size.has_value() && arr->elems.size() != size) {
            throw errors::wrong_args_count(size.value(), arr->elems.size(), line);
        }

        return arr;
    }

    template<typename T, typename Fn>
    std::vector<T> to_array_of(const Expr &expr, std::optional<size_t> size, int line, Fn fn) {
        auto arr = to_array(expr, size, line);
        std::vector<T> elem_values;

        for (auto &elem: arr->elems) {
            elem_values.push_back(fn(*elem, line));
        }

        return elem_values;
    }

    template<typename T, ExprType E>
    const T *to_expr(const Expr &expr, int line) {
        if (expr.type() != E)
            throw errors::type_mismatch(E, expr.type(), line);
        return dynamic_cast<const T *>(&expr);
    }

    const Number *to_num(const Expr &expr, int line) {
        return to_expr<Number, ExprType::number>(expr, line);
    }

    const Boolean *to_bool(const Expr &expr, int line) {
        return to_expr<Boolean, ExprType::boolean>(expr, line);
    }

    real to_real(const Expr &expr, int line) {
        return to_num(expr, line)->value;
    }

    const String *to_str(const Expr &expr, int line) {
        return to_expr<String, ExprType::string>(expr, line);
    }

    std::vector<double> to_num_array(const Expr &expr, size_t size, int line) {
        return to_array_of<double>(expr, size, line, to_real);
    }

    PointLight to_point_light(const Expr &expr, int line) {
        auto object = to_object(expr, line);
        PointLight light;

        for (auto &field: object->fields) {
            SKIP_DOC_FIELDS;
            if (field.key() == "position") light.position = to_point(*field.value_, field.line);
            else if (field.key() == "intensity") light.intensity = to_color(*field.value_, field.line);
            else throw_unknown_field_error(field);
        }

        return light;
    }

    void throw_unknown_field_error(const Field &field) {
        throw errors::unknown_field(field.key().value, field.line);
    }
}