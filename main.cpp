#include <iostream>
#include <fstream>
#include <ctime>

int main() {
    std::ofstream out_img{"output.ppm"};

    if (!out_img) {
        std::cerr << "Unable to open the output file!\n";
        return 1;
    }

    std::cout << "Rendering...\n";

    auto start_time = clock();

    // TODO: Add the rendering logic here. The ray tracer is still in-progress. For now we only have the tests.

    auto duration = static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC;

    std::cout << "Done. Running time: " << duration << " seconds";

    return 0;
}
