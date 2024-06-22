cmake_minimum_required(VERSION 3.10)

# Project Name
project(AutonomousVehicle)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files
set(SOURCES
    src/main.cpp
    src/LogManager.cpp
    src/SensorDataProcessor.cpp
    src/PathPlanar.cpp
    src/VehicleController.cpp
)

# Include directories
include_directories(src)

# Find and link required libraries
find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)

# Add the executable
add_executable(AutonomousVehicle ${SOURCES})

# Link libraries
target_link_libraries(AutonomousVehicle Boost::boost nlohmann_json::nlohmann_json)

# Copy config file to build directory
file(COPY ${CMAKE_SOURCE_DIR}/config.json DESTINATION ${CMAKE_BINARY_DIR})
