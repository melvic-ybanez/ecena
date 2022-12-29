#include <iostream>
#include <fstream>
#include <ctime>

#include "dsl/include/lexer.h"
#include "dsl/include/parser.h"
#include "dsl/include/errors.h"
#include "dsl/include/eval.h"
#include "engine/include/renderer.h"

int main() {
    std::ofstream out_img{"output.ppm"};

    if (!out_img) {
        std::cerr << "Unable to open the output file!\n";
        return 1;
    }

    std::string source;
    std::string line;

    while (getline(std::cin, line)) {
        source += line + "\n";
    }

    std::unique_ptr<rt::dsl::Object> object;

    rt::dsl::Lexer lexer{source};
    try {
        auto tokens = lexer.scan_all();
        rt::dsl::Parser parser{tokens};
        object = parser.parse_object();
    } catch (rt::dsl::errors::Error &error) {
        std::cout << error;
        return 1;
    }

    auto data = rt::dsl::eval::to_data(*object);

    std::cout << "Rendering...\n";

    out_img << rt::render(data);

    auto start_time = clock();

    auto duration = static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC;

    std::cout << "Done. Running time: " << duration << " seconds";

    return 0;
}
