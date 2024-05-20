#include "FluidSimulationApp.h"

int main(int argc, char** argv) {
    std::cout << "Starting Fluid Simulation App..." << std::endl;
    FluidSimulationApp app(argc, argv);
    app.Run();
    std::cout << "Exiting Fluid Simulation App..." << std::endl;
    return 0;
}
