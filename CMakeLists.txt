cmake_minimum_required(VERSION 3.21)
project(PhysiSim)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(PhysiSim 
    src/main.cpp
    src/Physics/Simulations/Collisions.cpp
    src/Physics/Objects/PhysicsObject.cpp
    src/Physics/Objects/Circle.cpp
    src/Physics/SpatialMap.cpp
    src/Physics/Constants.cpp
    src/UI/Button.cpp
    src/UI/Slider.cpp
)
target_link_libraries(PhysiSim SFML::Graphics SFML::Window SFML::System)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})