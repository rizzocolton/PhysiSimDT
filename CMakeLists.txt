cmake_minimum_required(VERSION 3.21)
project(PhysiSim)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(PhysiSim main.cpp)
target_link_libraries(PhysiSim SFML::Graphics SFML::Window SFML::System)
