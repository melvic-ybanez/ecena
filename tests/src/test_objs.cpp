//
// Created by Melvic Ybanez on 2/5/23.
//

#include "../include/tests.h"
#include "../include/asserts.h"
#include "../../engine/include/obj.h"

namespace rt::tests::objs {
    static void errors();

    static void vertices();

    static void faces();

    static void groups();

    void test() {
        set("Objs", [] {
            errors();
            vertices();
            faces();
            groups();
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
            auto [obj, ignored_lines] = obj::Parser::parse_verbose(obj_str);
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
            auto obj = obj::Parser::parse(obj_str);
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
            auto obj = obj::Parser::parse(obj_str);
            auto& t1 = obj.triangle_at(1);
            auto& t2 = obj.triangle_at(2);

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
            auto obj = obj::Parser::parse(obj_str);
            auto& t1 = obj.triangle_at(1);
            auto& t2 = obj.triangle_at(2);
            auto& t3 = obj.triangle_at(3);

            shapes::Triangle t1_expected{obj.vertex_at(1), obj.vertex_at(2), obj.vertex_at(3)};
            shapes::Triangle t2_expected{obj.vertex_at(1), obj.vertex_at(3), obj.vertex_at(4)};
            shapes::Triangle t3_expected{obj.vertex_at(1), obj.vertex_at(4), obj.vertex_at(5)};
            ASSERT_EQ_MSG("T1", t1_expected, t1);
            ASSERT_EQ_MSG("T2", t2_expected, t2);
            ASSERT_EQ_MSG("T3", t3_expected, t3);
        });
    }

    void groups() {
        auto make_stream = []() -> std::stringstream {
            return std::stringstream{
                    "v -1 1 0\n"
                    "v -1 0 0\n"
                    "v 1 0 0\n"
                    "v 1 1 0\n"
                    "\n"
                    "g FirstGroup\n"
                    "f 1 2 3\n"
                    "g SecondGroup\n"
                    "f 1 3 4"
            };
        };

        set("Triangles in groups", [&] {
            auto obj_str = make_stream();
            auto obj = obj::Parser::parse(obj_str);
            auto& t1 = obj.triangle_at(1, "FirstGroup");
            auto& t2 = obj.triangle_at(1, "SecondGroup");

            shapes::Triangle t1_expected{obj.vertex_at(1), obj.vertex_at(2), obj.vertex_at(3)};
            shapes::Triangle t2_expected{obj.vertex_at(1), obj.vertex_at(3), obj.vertex_at(4)};
            ASSERT_EQ_MSG("T1", t1_expected, t1);
            ASSERT_EQ_MSG("T2", t2_expected, t2);
        });

        set("Converting an OBJ file to a group", [&] {
            auto obj_str = make_stream();
            auto obj = obj::Parser::parse(obj_str);
            auto group = obj.to_group();
            shapes::NamedGroup first_group{"FirstGroup"};
            shapes::NamedGroup second_group{"SecondGroup"};
            ASSERT_TRUE_MSG("First group", group->contains_val(first_group));
            ASSERT_TRUE_MSG("Second group", group->contains_val(second_group));
        });
    }
}