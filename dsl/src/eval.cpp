//
// Created by Melvic Ybanez on 12/27/22.
//

#include "../include/eval.h"
#include "../include/errors.h"

namespace rt::dsl::eval {
    static Point to_point(const std::unique_ptr<Expr> &expr, int line);

    static math::Dimensions to_dimensions(const std::unique_ptr<Expr> &expr, int line);

    static Array *to_array(const std::unique_ptr<Expr> &expr, std::optional<size_t> size_limit, int line);

    static std::vector<double> to_num_array(const std::unique_ptr<Expr> &expr, size_t size_limit, int line);

    static Object *to_object(const std::unique_ptr<Expr> &expr, int line);

    Data to_data(const Object &object) {
        Data data;

        for (auto &field: object.fields) {
            if (field.key() == "ray_origin") data.ray_origin = to_point(field.value_, field.line);
            if (field.key() == "wall") data.wall = to_wall(field.value_, field.line);
            if (field.key() == "canvas") {
                auto dims = to_dimensions(field.value_, field.line);
                data.canvas = {static_cast<int>(dims.width), static_cast<int>(dims.height)};
            }
            if (field.key() == "shapes") data.shapes = to_shapes(field.value_, field.line);
        }

        return data;
    }

    std::vector<std::unique_ptr<Shape>> to_shapes(const std::unique_ptr<Expr> &expr, int line) {
        auto arr = to_array(expr, {}, line);
        std::vector<std::unique_ptr<Shape>> shapes;

        for (auto &elem: arr->elems) {
            if (auto shape = to_shape(elem, line); shape != nullptr) {
                shapes.push_back(std::move(shape));
            }
        }

        return shapes;
    }

    std::unique_ptr<Shape> to_shape(const std::unique_ptr<Expr> &expr, int line) {
        auto obj = to_object(expr, line);
        std::unique_ptr<Shape> shape;

        for (auto &field: obj->fields) {
            if (field.key() == "type") {
                if (field.value().type() != ExprType::string)
                    throw errors::type_mismatch(ExprType::string, field.value().type(), line);
                auto type = dynamic_cast<String *>(field.value_.get());
                if (*type == "sphere") shape = std::make_unique<shapes::Sphere>();
            }
            if (field.key() == "material") {
                shape->material = to_material(field.value_, line);
            }
        }

        return shape;
    }

    Material to_material(const std::unique_ptr<Expr> &expr, int line) {
        auto obj = to_object(expr, line);
        Material material;

        for (auto &field: obj->fields) {
            if (field.key() == "color") {
                auto point = to_point(field.value_, line);
                material.color = Color{point.x(), point.y(), point.z()};
            }
        }

        return material;
    }

    Wall to_wall(const std::unique_ptr<Expr> &expr, int line) {
        Wall wall;

        auto obj = to_object(expr, line);

        for (auto &field: obj->fields) {
            if (field.key() == "location") wall.location = to_point(field.value_, field.line);
            if (field.key() == "size") wall.size = to_dimensions(field.value_, field.line);
        }

        return wall;
    }

    Object *to_object(const std::unique_ptr<Expr> &expr, int line) {
        if (expr->type() != ExprType::object) {
            throw errors::type_mismatch(ExprType::object, expr->type(), line);
        }

        return dynamic_cast<Object *>(expr.get());
    }

    Point to_point(const std::unique_ptr<Expr> &expr, int line) {
        auto elem_values = to_num_array(expr, 3, line);
        return Point{elem_values[0], elem_values[1], elem_values[2]};
    }

    math::Dimensions to_dimensions(const std::unique_ptr<Expr> &expr, int line) {
        std::vector<int> elem_values;
        for (auto elem: to_num_array(expr, 2, line)) {
            auto int_elem = static_cast<int>(elem);
            if (elem != int_elem) {
                throw errors::type_mismatch("whole number", "floating point", line);
            }
            elem_values.push_back(int_elem);
        }
        return math::Dimensions{elem_values[0], elem_values[1]};
    }

    Array *to_array(const std::unique_ptr<Expr> &expr, std::optional<size_t> size_limit, int line) {
        if (expr->type() != ExprType::array) {
            throw errors::type_mismatch(ExprType::array, expr->type(), line);
        }

        auto arr = dynamic_cast<Array *>(expr.get());

        if (size_limit.has_value() && arr->elems.size() != size_limit) {
            throw errors::wrong_args_count(size_limit.value(), arr->elems.size(), line);
        }

        return arr;
    }

    std::vector<double> to_num_array(const std::unique_ptr<Expr> &expr, size_t size_limit, int line) {
        auto arr = to_array(expr, size_limit, line);
        std::vector<double> elem_values;

        for (auto &elem: arr->elems) {
            if (elem->type() != ExprType::number)
                throw errors::type_mismatch(ExprType::number, elem->type(), line);
            elem_values.push_back(dynamic_cast<Number *>(elem.get())->value);
        }

        return elem_values;
    }
}