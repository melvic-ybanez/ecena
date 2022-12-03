#include <iostream>
#include "include/image.h"
#include "include/color.h"
#include <ctime>

int main() {
    std::ofstream out_img{"output.ppm"};

    if (!out_img) {
        std::cerr << "Unable to open the output file!\n";
        return 1;
    }

    out_img << "P3\n";
    out_img << image::width << ' ' << image::height << "\n255\n";

    std::cout << "Rendering...\n";

    auto start_time = clock();

    for (int j = image::height - 1; j >= 0; j--) {
        for (int i = 0; i < image::width; i++) {
            auto r = static_cast<double>(i) / (image::width - 1);
            auto g = static_cast<double>(j) / (image::height - 1);
            auto b = 0.25;

            write_color(out_img, {r, g, b});
        }
    }

    auto duration = static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC;

    std::cout << "Done. Running time: " << duration << " seconds";

    return 0;
}
