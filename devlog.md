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

1. Created a base PhysicsObject Class to be parent class of all objects
2. Created a subclass, Circle, for circular object
3. Got objects to have positions updated by velocities and forces, so added gravity as well
4. Made boundary collision particular to PhysicsObject type
5. Separated PhysicsObject and Circle into their own file(s)

## Making Collisions

1. Implemented a collision method into the circle class
    * Checks if circles are intersecting
    * If yes, uses vector mathematics to compute velocities parallel to line of impact/perpendicular to plane of contact
    * Then uses conservation of energy and momentum to compute resultant velocities along line of impact
    * Then recombines to form and assign new velocities
2. Ran into issues making collisions functional... lots of clipping and annoying problems with top left origin circles
3. Fixed origin issues but now circles are being attracted when they touch, not bouncing...
4. After about a minute of thinking, I've realized it's because I need to change the sign when computing one of the circles impact velocity.
    * as of now, the direction of collision is faced one particular direction and both circles are giving components along it (in the same direction)
