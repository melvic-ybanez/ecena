//
// Created by Melvic Ybanez on 12/27/22.
//

#include "../include/eval.h"
#include "../include/errors.h"

namespace rt::dsl::eval {
    Data object(const Object &object) {
        Data data;

        for (auto &field: object.fields) {
            if (field.key() == "ray_origin") to_ray_origin(field, data);
            // TODO: Parse the rest of the field keys here...
        }

        return data;
    }

    void to_ray_origin(const Field &field, Data &data) {
        if (field.value().type() == ExprType::array) {
            auto point_data = dynamic_cast<Array *>(field.value_.get());
            std::vector<double> elem_values;

            for (auto &elem: point_data->elems) {
                if (elem->type() != ExprType::number)
                    throw errors::type_mismatch(ExprType::number, elem->type(), field.line);
                elem_values.push_back(dynamic_cast<Number *>(elem.get())->value);
            }
            if (elem_values.size() != 3) {
                throw errors::wrong_args_count(3, elem_values.size(), field.line);
            }

            data.ray_origin = Point{elem_values[0], elem_values[1], elem_values[2]};
        } else throw errors::type_mismatch(ExprType::array, field.value().type(), field.line);
    }
}