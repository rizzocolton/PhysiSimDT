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
5. After a lot of fiddling, I saw that sfml already has a "projectedOnto" function which does the behavior I've described. Only thing is I still need that one negative for the impact velocity... strange... I'm going to test that out a bit.
6. Realized that my velocity calculations were actually wrong, had some signs off explaining the whole negative sign creating almost normal behavior thing. As it turns out the projection doesn't do any weird sign correction thing and behaves as you would think it would.
7. I've deemed that clipping is more accurate than adjusting positions of intersecting circles, so I removed the position correction formula and added a velocity check formula instead (that way collisions don't happen if objects are already moving away from eachother).

## Making Collisions More Efficient

1. Added an fps counter to see how efficiently my simulation is running
2. Corrected floor clipping along the way, basically a circle needs to be outside of bounds and actively moving outside of bounds to have a component flipped
3. Started work on a hashmap to contain each object in pockets of space
4. Ran into a huge amount of issues with finding a good key to use that hashes in the unordered_map
5. Ended up having to make my own GridKey struct and hash to work in the unordered_map
6. Got displaying the spatial map down using sf::RectangleShape
7. Added features for circles entering and exiting cells in spatial map
8. Decided to rework that strategy and instead have circles enter their cell every frame
9. Finally got spatial map working and fully functional

Overall I had very impressive performance gains on this. Went from 43 fps at 2000 objects for pre-spatial map implementation, now it's at 430 fps for 2000 objects with a spatial map.

## Creating UI

1. Started with start/pause with states (normal, hovered, clicked)
2. Progressed to sliders
3. Made gravity and res sliders
4. Made separate window with detail view of an object
5. Made adjustable mass and radius for that particular object

## Refactoring For One Window

1. Figured that 3 windows would be clunky and unoptimized
2. Decided to restart main.cpp with a Simulation class base
3. After a single window mode was functional, moved on to some more organizational fixes before fully refactoring all UI/interactions
3. Reorganized storage of simulation objects to be more generic using a vector of unique pointers (for the sweet polymorphism)
4. Reogranized storage of ui objects accordingly

While finicking around with ui and testing I realized a great way to optimize cell checks for collisions
each object should only check its own cell and the cells to the right, down right, down, and down left from itself. 
every celll only once checks every other cell adjacent once


## Revisiting Devlog After Significant Development

Unfortunately I have made a lot of changes since the last time I updated this page without making any new updates here.
I'll just quickly go over some of the biggest developments:
* Made an E&M mode and a Gravity mode
* Made UI initializations separate files for every simulation subclass
* Switched from frame dependent to frame independent timesteps
    * Basically I now use a fixed timestep instead of relying on the amount of time it takes for the program to complete a frame to dictate the next frames dt
    * Much more deterministic sims, plus a lot more accurate at faster sim speeds
    * This has however significantly impacted performance for high object counts, 10k objects is somewhat infeasible atp

## Optimizing Performance in Collision Sim

Before I refactor the other sims to use the frame independent calculations I need to go over the performance in collision sim now. It's a weird question of how to actually measure performance whenever I can lower the sim speed and have a higher fps due to the frame independence. What I can say for certain though is that this move has made *real time* performance for 10k objects drop significantly. I can only get about 15 fps with a similar sized spatial grid sizing as before. I'm going to do some profiling of my collisions update to see if there's anything I can optimize.

So right now at 10k objects, real time:
Accumulator Cycle: 70-100ms
Collision Physics Update: 6-8ms
Object Rendering: 7-10ms

Looking at those numbers I think I'm just going to cap the iterations for the accumulator. This means that my sim will lag behind at real

## Refactoring Sim Units

Did massive refactoring to use SI units in all physics calculations leaving pixels for rendering and the spatial map.

## Refactoring for ECS

Completely rebuilt using a different design philosophy. Will be putting performance metrics here:

### 10k objects w/no collision affected by uniform gravitational field with reflective boundary, Euler Integration:

* Computation Time Per Time-Step: ~100 microseconds
* Energy Leakage: -0.191 J/s

## Same as above but with Velocity Verlet integration:

* Computation Time Per Time-Step: ~100 microseconds
* Energy Leakage: 0 J/s

Using velocity Verlet integration literally deleted all energy leakage, at least for this simple system. Wow. I can't believe I've been using Euler integration for so long until this point and wondering where all of the energy in my system went!

