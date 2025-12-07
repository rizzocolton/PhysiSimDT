## Making The Environment

1. started with installing the vcpkg package manager.
2. ran the bootstrap script inside the new vcpkg directory.
3. ran ./vcpkg integrate install to integrate vcpkg with CMake. 
4. ran ./vcpkg install sfml to install the sfml graphics library.

## Getting The Display

1. Went through many hoops and hurdles to get CMake to recognize SFML.
2. Finally got a simple screen with a circle set up.
3. A secondary display for controls was also added.

## Creating First Objects

1. Created a base PhysicsObject Class to be a superclass of all objects
2. Created a subclass, Circle, for circular object
3. Got objects to have positions updated by velocities and forces, so added gravity as well
4. Made boundary collision particular to PhysicsObject type

