cmake_minimum_required(VERSION 3.21)
project(PhysiSim)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(PhysiSim 
    src/main.cpp
    src/Physics/Systems.cpp
    src/Physics/Simulations/Collisions.cpp
    src/UI/SimInits/CollisionsUI.cpp
    src/Physics/Simulations/Menu.cpp
    src/UI/SimInits/MenuUI.cpp
    src/Physics/SpatialHashMap.cpp
    src/UI/Button.cpp
    src/UI/Slider.cpp
    src/UI/Spinner.cpp
    src/UI/Label.cpp
)
target_link_libraries(PhysiSim SFML::Graphics SFML::Window SFML::System)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})