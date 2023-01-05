//
// Created by Melvic Ybanez on 12/27/22.
//

#include "../include/eval.h"
#include "../include/errors.h"

namespace rt::dsl::eval {
    static Point to_point(const Expr &expr, int line);

    static Vec to_vec(const Expr &expr, int line);

    static Color to_color(const Expr &expr, int line);

    static const Array *to_array(const Expr &expr, std::optional<size_t> size, int line);

    static std::vector<double> to_num_array(const Expr &expr, size_t size, int line);

    static const Object *to_object(const Expr &expr, int line);

    static const Number *to_num(const Expr &expr, int line);

    static const String *to_str(const Expr &expr, int line);

    static real to_real(const Expr &expr, int line);

    static std::vector<Matrix<4, 4>> to_transforms(const Expr &expr, int line);

    Data to_data(const Object &object) {
        Data data;

        for (auto &field: object.fields) {
            if (field.key() == "camera") data.camera = to_camera(*field.value_, field.line);
            if (field.key() == "world") data.world = to_world(*field.value_, field.line);
        }

        return data;
    }

    Camera to_camera(const Expr &expr, int line) {
        auto obj = to_object(expr, line);
        Camera camera;

        for (auto &field: obj->fields) {
            if (field.key() == "h_size") camera.h_size = to_real(*field.value_, field.line);
            if (field.key() == "v_size") camera.v_size = to_real(*field.value_, field.line);
            if (field.key() == "field_of_view") camera.field_of_view = to_real(*field.value_, field.line);
            if (field.key() == "transform") {
                auto arr = to_array(*field.value_, 3, line);
                auto from = to_point(*arr->elems[0], line);
                auto to = to_point(*arr->elems[1], line);
                auto up = to_vec(*arr->elems[1], line);
                camera.transform = math::matrix::view_transform(from, to, up);
            }
        }

        return camera;
    }

    World to_world(const Expr &expr, int line) {
        auto obj = to_object(expr, line);
        World world;

        for (auto &field: obj->fields) {
            if (field.key() == "objects") world.objects = to_shapes(*field.value_, field.line);
            if (field.key() == "light") world.light = to_point_light(*field.value_, field.line);
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

        for (auto &field: obj->fields) {
            if (field.key() == "type") {
                auto type = to_str(*field.value_, field.line);
                if (*type == "sphere") shape = std::make_unique<shapes::Sphere>();
            }
            if (field.key() == "material") shape->material = to_material(*field.value_, line);
            if (field.key() == "transform") {
                auto transforms = to_transforms(*field.value_, line);
                std::reverse(transforms.begin(), transforms.end());
                for (auto t: transforms) {
                    shape->transformation = shape->transformation * t;
                }
            }
        }

        return shape;
    }

    std::unique_ptr<Material> to_material(const Expr &expr, int line) {
        auto obj = to_object(expr, line);
        auto material = std::make_unique<Material>();

        for (auto &field: obj->fields) {
            if (field.key() == "color") material->color = to_color(*field.value_, field.line);
            if (field.key() == "specular") material->specular = to_real(*field.value_, field.line);
            if (field.key() == "diffuse") material->diffuse = to_real(*field.value_, field.line);
        }

        return material;
    }

    std::vector<Matrix<4, 4>> to_transforms(const Expr &expr, int line) {
        auto arr = to_array(expr, {}, line);
        std::vector<Matrix<4, 4>> transform;

        for (auto &elem: arr->elems) {
            auto t = to_array(*elem, 2, line);
            auto func = to_str(*t->elems[0], line);
            if (*func == "scale") {
                auto values = to_num_array(*t->elems[1], 3, line);
                transform.push_back(matrix::scaling(values[0], values[1], values[2]));
            } else if (*func == "translate") {
                auto values = to_num_array(*t->elems[1], 3, line);
                transform.push_back(matrix::translation(values[0], values[1], values[2]));
            } else if (*func == "rotate_x") {
                auto r = to_real(*t->elems[1], line);
                transform.push_back(matrix::rotation_x(r));
            } else if (*func == "rotate_y") {
                auto r = to_real(*t->elems[1], line);
                transform.push_back(matrix::rotation_y(r));
            } else if (*func == "rotate_z") {
                auto r = to_real(*t->elems[1], line);
                transform.push_back(matrix::rotation_z(r));
            } else if (*func == "shearing") {
                auto values = to_num_array(*t->elems[1], 6, line);
                transform.push_back(matrix::shearing(values[0], values[1], values[2], values[3], values[4], values[5]));
            }
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

    const Number *to_num(const Expr &expr, int line) {
        if (expr.type() != ExprType::number)
            throw errors::type_mismatch(ExprType::number, expr.type(), line);
        return dynamic_cast<const Number *>(&expr);
    }

    real to_real(const Expr &expr, int line) {
        return to_num(expr, line)->value;
    }

    const String *to_str(const Expr &expr, int line) {
        if (expr.type() != ExprType::string)
            throw errors::type_mismatch(ExprType::string, expr.type(), line);
        return dynamic_cast<const String *>(&expr);
    }

    std::vector<double> to_num_array(const Expr &expr, size_t size, int line) {
        auto arr = to_array(expr, size, line);
        std::vector<double> elem_values;

        for (auto &elem: arr->elems) {
            elem_values.push_back(to_real(*elem, line));
        }

        return elem_values;
    }

    PointLight to_point_light(const Expr &expr, int line) {
        auto object = to_object(expr, line);
        PointLight light;

        for (auto &field: object->fields) {
            if (field.key() == "position") light.position = to_point(*field.value_, field.line);
            if (field.key() == "intensity") light.intensity = to_color(*field.value_, field.line);
        }

        return light;
    }
}