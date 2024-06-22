#include "LogManager.h"
#include "SensorDataProcessor.h"
#include "PathPlanar.h"
#include "VehicleController.h"
#include <vector>
#include <cstdlib>
#include <chrono>

void measurePerformance(LogManager& logger, void (*func)(std::vector<int>&), std::vector<int>& data, const std::string& moduleName) {
    auto start = std::chrono::high_resolution_clock::now();
    func(data);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    logger.logPerformance(moduleName, elapsed.count());
}

void processSensorData(std::vector<int>& data) {
    SensorDataProcessor processor;
    processor.process(data);
}

void planPath(std::vector<int>& path) {
    PathPlanar planner;
    planner.plan(path);
}

void controlVehicle(std::vector<int>& commands) {
    VehicleController controller;
    controller.control(commands);
}

int main() {
    LogManager logger("config.json");

    std::vector<int> sensorData(1000);
    std::generate(sensorData.begin(), sensorData.end(), std::rand);

    std::vector<int> pathData = sensorData;
    std::vector<int> controlData = sensorData;

    measurePerformance(logger, processSensorData, sensorData, "SensorDataProcessing");
    measurePerformance(logger, planPath, pathData, "PathPlanning");
    measurePerformance(logger, controlVehicle, controlData, "VehicleControl");

    logger.stop();
    return 0;
}
