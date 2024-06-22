#include "SensorDataProcessor.h"
#include <algorithm>
#include <thread>
#include <chrono>

void SensorDataProcessor::process(std::vector<int>& data) {
    // Simulate sensor data processing by sorting the data
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate processing delay
    std::sort(data.begin(), data.end());
}
