#include "PathPlanar.h"
#include <algorithm>
#include <thread>
#include <chrono>

void PathPlanar::plan(std::vector<int>& path) {
    // Simulate path planning by reversing the path
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Simulate planning delay
    std::reverse(path.begin(), path.end());
}
