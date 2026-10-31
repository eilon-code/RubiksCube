#include "RubiksCubeManager.hpp"
#include <iostream>

int main() {
    bool debug = false; // Set to true to enable debug output
    if (debug) {
        std::cout << "[DEBUG] Starting app..." << std::endl;
    }
    RubiksCubeManager game(1000, 600, "Rubik's Cube 3D");
    game.run(debug);
    return 0;
}
