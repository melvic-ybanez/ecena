//
// Created by Melvic Ybanez on 2/5/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/obj.h"

namespace rt::tests::objs {
    void errors();

    void vertices();

    void faces();

    void test() {
        set("Objs", [] {
            errors();
            vertices();
            faces();
        });
    }

    void errors() {
        scenario("Ignoring unrecognized lines", [] {
            std::stringstream obj_str{
                    "There was a young lady named Bright\n"
                    "who traveled much faster than light.\n"
                    "She set out one day\n"
                    "in a relative way,\n"
                    "and came back the previous night."
            };
            auto [obj, ignored_lines] = obj::parse_verbose(obj_str);
            ASSERT_EQ(5, ignored_lines);
        });
    }

    void vertices() {
        set("Vertex records", [] {
            std::stringstream obj_str{
                    "v -1 1 0\n"
                    "v -1.0000 0.5000 0.0000\n"
                    "v 1 0 0\n"
                    "v 1 1 0"
            };
            auto obj = obj::parse(obj_str);
            ASSERT_EQ_MSG("1st", Point(-1, 1, 0), obj.vertex_at(1));
            ASSERT_EQ_MSG("2nd", Point(-1, 0.5, 0), obj.vertex_at(2));
            ASSERT_EQ_MSG("3rd", Point(1, 0, 0), obj.vertex_at(3));
            ASSERT_EQ_MSG("4th", Point(1, 1, 0), obj.vertex_at(4));
        });
    }

    void faces() {
        set("Parsing triangle faces", [] {
            std::stringstream obj_str{
                    "v -1 1 0\n"
                    "v -1 0 0\n"
                    "v 1 0 0\n"
                    "v 1 1 0\n"
                    "\n"
                    "f 1 2 3\n"
                    "f 1 3 4"
            };
            auto obj = obj::parse(obj_str);
            auto &t1 = obj.triangle_at(1);
            auto &t2 = obj.triangle_at(2);

            shapes::Triangle t1_expected{obj.vertex_at(1), obj.vertex_at(2), obj.vertex_at(3)};
            shapes::Triangle t2_expected{obj.vertex_at(1), obj.vertex_at(3), obj.vertex_at(4)};
            ASSERT_EQ_MSG("T1", t1_expected, t1);
            ASSERT_EQ_MSG("T2", t2_expected, t2);
        });
        set("Triangulating polygons", [] {
            std::stringstream obj_str{
                "v -1 1 0\n"
                "v -1 0 0\n"
                "v 1 0 0\n"
                "v 1 1 0\n"
                "v 0 2 0\n"
                "\n"
                "f 1 2 3 4 5"
            };
            auto obj = obj::parse(obj_str);
            auto &t1 = obj.triangle_at(1);
            auto &t2 = obj.triangle_at(2);
            auto &t3 = obj.triangle_at(3);

            shapes::Triangle t1_expected{obj.vertex_at(1), obj.vertex_at(2), obj.vertex_at(3)};
            shapes::Triangle t2_expected{obj.vertex_at(1), obj.vertex_at(3), obj.vertex_at(4)};
            shapes::Triangle t3_expected{obj.vertex_at(1), obj.vertex_at(4), obj.vertex_at(5)};
            ASSERT_EQ_MSG("T1", t1_expected, t1);
            ASSERT_EQ_MSG("T2", t2_expected, t2);
            ASSERT_EQ_MSG("T3", t3_expected, t3);
        });
    }
}