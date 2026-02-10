# PhysiSim: A C++ Physics Engine & Interactive Simulator

A 2D physics simulation engine built from scratch in C++, designed as a learning project to learn modern C++ practices and real-time physics computation. PhysiSim provides multiple interactive physics simulations with an intuitive UI for real-time parameter adjustment and object inspection.

## Overview

PhysiSim is a computational physics engine that demonstrates core concepts including collision detection/response, spatial hashing for optimization, gravity simulation, and electromagnetic forces. The project showcases software engineering principles like polymorphism, efficient data structures, modular architecture and performance optimization through real-time rendering and complex physics calculations.

**Note:**  One of the key purposes of this project is to help me gain familiarity and experience with C++. Generative AI was used only for debugging during development, ocassional refactoring, and for developing parts of this readme.

## Features

### Physics Simulations

- **Collision Simulation**: Watch circles collide realistically with energy and momentum conservation
- **Gravity Simulation**: Simulate gravitational attraction between objects
- **Electromagnetic Simulation**: Explore charged particle behavior with electromagnetic forces
- **Menu System**: Navigate between different simulation modes

### Core Engine Capabilities

- **Realistic Physics**: 
  - Vector mathematics-based collision response
  - Energy and momentum conservation in collisions
  - Accurate velocity and position calculations using frame-dependent euler integration
  - Proportional gravitational and electromagnetic force calculations

- **Performance Optimization**:
  - Spatial hashing grid system for efficient collision detection
    - Spatial cells to reduce O(n²) collision checks
  - Real-time FPS counter and population display
  - Handles 10k objects at 60 FPS

- **Interactive UI**:
  - Play/Pause controls
  - Real-time parameter adjustment (gravity, restitution, mass, radius)
  - Object detail inspector for examining individual properties
  - Button and slider controls
  - Visual grid overlay for spatial partitioning

- **Visual Features**:
  - Full-screen rendering
  - Real-time FPS and population counters
  - Spatial grid visualization
  - Organzied control panel
  - Separate detail view window for object inspection

## Screenshots & Demo
- Collision Simulation Demo:

![Collision Simulation Demo](https://github.com/user-attachments/assets/ccd9dd96-8568-443b-b436-4519e2a9d32f)

## System Requirements

- **C++ Standard**: C++17 or later
- **Build System**: CMake 3.21+
- **Graphics Library**: SFML 3.x
- **Compiler**: MSVC or compatible C++17-supporting compiler
- **OS**: Windows (primary development target)

## Installation & Setup

### Prerequisites

1. **Install vcpkg** (Microsoft's package manager for C++)
   ```bash
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   ```

2. **Install SFML via vcpkg**
   ```bash
   .\vcpkg install sfml
   ```

### Building the Project

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd PhysiSim
   ```

2. **Create build directory**
   ```bash
   mkdir build
   cd build
   ```

3. **Configure and build with CMake**
   ```bash
   cmake ..
   cmake --build . --config Release
   ```

4. **Run the executable**
   ```bash
   .\PhysiSim.exe
   ```

## How to Use

### Main Menu
- Start the application to see the menu screen
- Use the "Start" button to launch the default collision simulation or navigate to other simulation types

### During Simulation

**Controls:**
- **Start/Pause Button**: Toggle the simulation between running and paused states
- **Menu Button**: Return to the main menu
- **Property Sliders**: Change simulation parameters
- **FPS Counter**: Top-left shows current frames per second
- **Population Counter**: Shows current number of objects in the simulation

### Selecting Objects
- Right-click on objects in the simulation to select and inspect them
- Selected object details appear in the bottom-left panel
- Modify mass and radius of selected objects in real-time

## Project Structure

```
PhysiSim/
├── src/
│   ├── main.cpp                          # Application entry point & main loop
│   ├── Physics/
│   │   ├── Objects/
│   │   │   ├── PhysicsObject.h/cpp       # Base class for physical objects
│   │   │   └── Circle.h/cpp              # Circle implementation with collision
│   │   ├── Simulations/
│   │   │   ├── Simulation.h              # Base simulation interface
│   │   │   ├── Collisions.h/cpp          # Collision simulation
│   │   │   ├── Gravity.h/cpp             # Gravity simulation
│   │   │   ├── EM.h/cpp                  # Electromagnetic simulation
│   │   │   └── Menu.h/cpp                # Menu simulation
│   │   ├── SpatialMap.h/cpp              # Spatial hashing grid system
│   │   └── GridKey.h                     # Hash key for spatial grid
│   ├── UI/
│   │   ├── UI.h                          # UI system interface
│   │   ├── Button.h/cpp                  # Interactive button component
│   │   ├── Slider.h/cpp                  # Parameter slider component
│   │   ├── Spinner.h/cpp                 # Numeric spinner component
│   │   ├── Label.h/cpp                   # Text label component
│   │   └── SimInits/
│   │       ├── CollisionsUI.cpp          # Collision simulation UI setup
│   │       ├── GravityUI.cpp             # Gravity simulation UI setup
│   │       ├── EMUI.cpp                  # EM simulation UI setup
│   │       └── MenuUI.cpp                # Menu UI setup
├── assets/                               # Fonts and other assets
├── CMakeLists.txt                        # Build configuration
├── README.md                             # This file
├── devlog.md                             # Development log
└── PhysiSim.code-workspace               # VS Code workspace configuration
```

## Technical Architecture

### Physics Engine

**Collision Detection & Response:**
- Uses circle-to-circle collision detection (distance comparison)
- Employs vector mathematics to resolve collision velocity components
- Separates velocity into parallel and perpendicular components relative to collision normal
- Applies conservation of momentum and energy to collision resolution
- Velocity checks prevent objects from "sticking" together

**Spatial Optimization:**
- Objects are partitioned into a grid of cells
- Only objects in the same cell or adjacent cells are checked for collisions
- Each cell only checks rightward, downward, and diagonal cells to avoid duplicate checks
- Reduces collision checks from O(n²) to ~O(n)
- Enables real-time simulation of 10k+ objects

**Physics Calculations:**
- Euler integration for position and velocity updates
- Frame-independent physics calculating a change in time from the system clock
- Boundary collision detection with configurable restitution
- Gravity applied as constant downward acceleration in Collisions mode
- Electromagnetic and gravitational forces based on inverse square Newtonian/Coulombic physics

### UI System

- Polymorphic UI component system (Button, Slider, Spinner, Label)
- Event-driven interaction handling
- Real-time mouse tracking for hover/click detection
- Per-simulation UI configuration through specialized initializers

## Performance Metrics

- **Pre-Optimization**: 40 fps with 2000 objects
- **Post-Spatial-Hashing**: 360 fps with 2000 objects, 60 fps for 10k objects
- **Performance Gain**: ~9x improvement through spatial grid optimization

## Development Notes

- Built as a learning project for C++ experience
- Utilizes modern C++ features (smart pointers, lambdas, unique_ptr polymorphism)

For detailed development timeline and technical decisions, see [devlog.md](devlog.md).

## Future Enhancements

- Additional physics types (magnets, field visualizations, springs, fluids)
- Keyboard input refinement
- More flexible object types (rectangles, polygons)
- Graphical analysis of properties over time/space

## Contact & Contributions

This is a personal learning project. Feel free to fork and adapt for your own learning purposes!
