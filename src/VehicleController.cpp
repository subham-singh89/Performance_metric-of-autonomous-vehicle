#include "VehicleController.h"
#include <thread>
#include <chrono>

void VehicleController::control(std::vector<int>& commands) {
    // Simulate vehicle control by simply sleeping
    std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Simulate control delay
}
