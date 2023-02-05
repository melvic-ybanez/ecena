//
// Created by Melvic Ybanez on 2/5/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/obj.h"

namespace rt::tests::objs {
    void errors();

    void vertices();

    void test() {
        set("Objs", [] {
            errors();
            vertices();
        });
    }

    void errors() {
        scenario("Ignoring unrecognized lines", [] {
            std::stringstream obj_str{"There was a young lady named Bright\n"
                                      "who traveled much faster than light.\n"
                                      "She set out one day\n"
                                      "in a relative way,\n"
                                      "and came back the previous night."};
            auto [obj, ignored_lines] = obj::parse_verbose(obj_str);
            ASSERT_EQ(5, ignored_lines);
        });
    }

    void vertices() {
        set("Vertex records", [] {
            std::stringstream obj_str{"v -1 1 0\n"
                                      "v -1.0000 0.5000 0.0000\n"
                                      "v 1 0 0\n"
                                      "v 1 1 0"};
            auto result = obj::parse(obj_str);
            ASSERT_EQ_MSG("1st", Point(-1, 1, 0), result[1]);
            ASSERT_EQ_MSG("2nd", Point(-1, 0.5, 0), result[2]);
            ASSERT_EQ_MSG("3rd", Point(1, 0, 0), result[3]);
            ASSERT_EQ_MSG("4th", Point(1, 1, 0), result[4]);
        });
    }
}