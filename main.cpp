#include <iostream>
#include <fstream>

#include "engine/include/renderer.h"
#include "dsl/include/interpreter.h"

int main() {
    std::ofstream out_img{"output.ppm"};

    if (!out_img) {
        std::cerr << "Unable to open the output file!\n";
        return 1;
    }

    auto source = rt::dsl::read_source();
    auto data = rt::dsl::interpret(source);

    if (!data.has_value()) return 1;

    rt::render_with_logging(data.value(), out_img);

    return 0;
}
