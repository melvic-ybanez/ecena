//
// Created by Melvic Ybanez on 12/26/22.
//

#include <iostream>
#include "../include/renderer.h"
#include <fstream>

namespace rt {
    Ppm render(Data& data) {
        return Ppm{data.camera.render(data.world)};
    }

    void render_with_logging(Data& data, std::ofstream& out_img) {
        std::cout << "Rendering...\n";

        auto start_time = clock();

        out_img << rt::render(data);

        auto duration = static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC;

        std::cout << "Done. Running time: " << duration << " seconds";
    }
}