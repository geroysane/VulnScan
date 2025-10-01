cmake_minimum_required(VERSION 3.16)

project(VulnScan VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Network Sql Widgets Test)

# Enable testing
enable_testing()

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src/gui)
add_subdirectory(src/cli)
add_subdirectory(tests)
