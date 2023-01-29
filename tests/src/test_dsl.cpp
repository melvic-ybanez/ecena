//
// Created by Melvic Ybanez on 1/29/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../dsl/include/ast.h"

namespace rt::tests::dsl_ {
    static void ast_types();

    void test() {
        set("DSL", [] {
            ast_types();
        });
    }

    void ast_types() {
        set("Convert AST Types to string", [] {
            ASSERT_EQ_MSG("Object", "Object", dsl::type_to_str(dsl::ExprType::object));
            ASSERT_EQ_MSG("Number", "Number", dsl::type_to_str(dsl::ExprType::number));
            ASSERT_EQ_MSG("String", "String", dsl::type_to_str(dsl::ExprType::string));
            ASSERT_EQ_MSG("Array", "Array", dsl::type_to_str(dsl::ExprType::array));
            ASSERT_EQ_MSG("Boolean", "Boolean", dsl::type_to_str(dsl::ExprType::boolean));
            ASSERT_EQ_MSG("Null", "Null", dsl::type_to_str(dsl::ExprType::null));
        });
    }
}