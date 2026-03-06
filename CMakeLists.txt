cmake_minimum_required(VERSION 3.21)
project(PhysiSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0
)
find_package(OpenMP REQUIRED)

FetchContent_MakeAvailable(SFML)

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
target_link_libraries(PhysiSim PRIVATE OpenMP::OpenMP_CXX sfml-graphics sfml-window sfml-system)
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

