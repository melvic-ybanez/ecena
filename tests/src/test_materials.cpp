//
// Created by Melvic Ybanez on 12/23/22.
//

#include "../include/test_materials.h"
#include "../include/asserts.h"
#include "../../core/include/material.h"

namespace tests::materials {
    void all() {
        set("Materials", [] {
            init();
        });
    }

    void init() {
        set("Default", [] {
            rt::Material mat;
            ASSERT_EQ_MSG("Color", rt::Color::white_, mat.color);
            ASSERT_EQ_MSG("Ambient", 0.1, mat.ambient);
            ASSERT_EQ_MSG("Diffuse", 0.9, mat.diffuse);
            ASSERT_EQ_MSG("Specular", 0.9, mat.specular);
            ASSERT_EQ_MSG("Shininess", 200, mat.shininess);
        });
    }
}