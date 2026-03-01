cmake_minimum_required(VERSION 3.21)
project(PhysiSim)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(PhysiSim 
    src/main.cpp
    src/Physics/Systems.cpp
    src/ObjectRenderer.cpp
    src/Physics/Simulations/Collisions.cpp
    src/UI/SimInits/CollisionsUI.cpp
    src/Physics/Simulations/Menu.cpp
    src/UI/SimInits/MenuUI.cpp
    src/Physics/SpatialHash.cpp
    src/UI/Button.cpp
    src/UI/Slider.cpp
    src/UI/Spinner.cpp
    src/UI/Label.cpp
)
target_link_libraries(PhysiSim SFML::Graphics SFML::Window SFML::System OpenMP::OpenMP_CXX)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # For Windows / Visual Studio
    target_compile_options(PhysiSim PRIVATE 
        /O2             # Turn on maximum speed optimizations
        /Qvec-report:2  # Tell MSVC to report vectorized loops
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # For Linux / Mac / MinGW
    target_compile_options(PhysiSim PRIVATE 
        -O3                       # Turn on maximum speed & auto-vectorization
        -fopt-info-vec-optimized  # Tell GCC/Clang to report successes
        # -fopt-info-vec-missed   # (Optional) Uncomment to see what FAILED to vectorize
    )
endif()

