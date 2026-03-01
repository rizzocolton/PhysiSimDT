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
* Accumulator Cycle: 70-100ms
* Collision Physics Update: 6-8ms
* Object Rendering: 7-10ms

Looking at those numbers I think I'm just going to cap the iterations for the accumulator. This means that my sim will lag behind at real

## Refactoring Sim Units

Did massive refactoring to use SI units in all physics calculations leaving pixels for rendering and the spatial map.

## Refactoring for ECS

Completely rebuilt using a different design philosophy. Will be putting performance metrics here:

### 10k objects w/no collision affected by uniform gravitational field with reflective boundary, Euler Integration:

* Computation Time Per Time-Step: ~100 microseconds
* Energy Leakage: -0.191 J/s

## Same as above but with Velocity Verlet integration:

* Computation Time Per Time-Step: ~130 microseconds
* Energy Leakage: 0 J/s

Using velocity Verlet integration literally deleted all energy leakage, at least for this simple system. Wow. I can't believe I've been using Euler integration for so long until this point and wondering where all of the energy in my system went!

## Same parameters but comparing render times before and after optimizing drawing

After using textures + vertex arrays:

* Object Rendering: ~400 microseconds

With 10k objects thats a crazy 17x improvement!!!

## Now before doing anymore optimizations, let's test this version's limits!

So keep in mind we're doing velocity verlet integration, optimized vertex array for rendering, random dispersion of objects. Also I had made the boundary collision detection more elaborate for circles which may explain the jump in physics update time.

10k Objects:
* Physics Update: ~190 microseconds
* Object Rendering: ~400 microseconds

20k Objects:
* Physics Update: ~280 microseconds
* Object Rendering: ~700 microseconds

50k Objects:
* Physics Update: ~560 microseconds
* Object Rendering: ~1820 microseconds

100k Objects:
* Physics Update: ~930 microseconds
* Object Rendering: ~3360 microseconds

500k Objects:
* Physics Update: ~6460 microseconds
* Object Rendering: ~17800 microseconds

The physics engine finally started running behind only at 500k objects. 

### This is an astronomical improvement over the original PhysiSim!!!

Now I'm going to implement collisions, and we'll unfortunately see those physics update numbers drop...

Collisions implemented. Now I'm going to check physical accuracy.

# Collision Detection/Response Algorithm Testing

### Trial 1

#### Inputs

Circle1:
* pos: [2, 2]
* vel: [4, 0]
* radius: 1
* mass: 1

Circle2:
* pos:[5, 2]
* vel:[0, 0]
* radius: 1
* mass: 1

#### Expected Output

Circle 1:
* vel: [0, 0]

Circle 2:
* vel: [4, 0]

#### Results

Checking Energy: Good!

Perfect! Stable even with multiple iterations (circles bouncing off boundary)

### Trial 2

#### Inputs

Circle 1:
* pos: [2, 3]
* vel: [2, 0]
* mass: 1
* radius: 1

Circle 2:
* pos: [5, 2]
* vel: [0, 0]
* mass: 1
* radius: 1

#### Expected Output

Glancing blows should cause a 90 degree separation, so the dot product of the resultant velocities should be zero.

#### Results

Checking Energy: Good!

Circle 1 Velocity: (0.506721, 0.869871)
Circle 2 Velocity: (1.49328, -0.869871)

[0.506721, 0.869871] dot [1.49328, -0.869871] = ~0, Success!


### Trial 3



#### Inputs

This one will be a little different. We'll set gravity to -9.81, keep collision restitution to 1.0 and put a circle with mass of 1 on top of an identical circle with mass of infinity (invmass=0) to see if there's any energy leak

Circle 1:
* pos: [5, 1]
* vel: [0, 0]
* mass: infinite
* radius: 1

Circle 2:
* pos: [5, 5]
* vel: [0, 0]
* mass: 1
* radius: 1

#### Results

No energy leak with this, everything seems stable...but lets try with a reduced collision restitution of .8

There's sinking now. I think I'll try implementing a mass weighted position correction.

Less sinking but still seeing energy generation with many particles, investigating boundary collision now.

Realized that i had a error where i was pushing objects away by their total combined radii not just half of it.

Realized that i was using the total difference in position to correct position, should've been using radius-diff. Now things look and behave much more predictably. Moving on to testing energy conservation in large groups again.

Looks like boundary collision is being screwy now. I'm thinking of modifying it to use a "time in barrier" calculation.

Fantastic results from using that method. Just need to watch out for particles spawning in the barrier... calculating a time inside barrier while already inside appears to give them infinite energy...

# Performance

All tests are done using randomly dispersed objects with random velocities and masses of 1 and radii of 2 pixels.
Collision and Boundary restitutions are 1, gravity is 0.

10 Objects:
* Total Physics Update: ~16 microseconds
* Collision Check/Response: ~1 microsecond

Granted, at least low object counts the computations are mostly just collision detection...

100 Objects:
* Total Physics Update: ~42 microseconds
* Collision Check/Response: ~18 microseconds

1k Objects:
* Total Physics Update: ~1700 microseconds
* Collision Check/Response: ~1600 microseconds

Looks like at this point the collision check and response starts to take a majority of the time frame.
Still, these numbers aren't awful... let's see what happens at 10k objects. This will surely scale horribly with O(n^2) checks...

10k Objects:
* Total Physics Update: ~138000 microseconds
* Collision Check/Response: ~138000 microseconds

Yikes, that's a tenth of an actual second to go through that. Let's see how much we can improve this performance with some sort of spatial hashing.

# Collision Detection/Response Algorithm Testing With Spatial Hashing


## Key features

Entity Component System Design

Velocity Verlet Integration

Multi-Threading* & SIMD

Conservation of Energy* & Momentum

Spatial Hashing*

*Tentative

## To Do

Look into GJK distance/ SaT for polygons.

Look into Continuous Collision Detection for collision checks at high speeds.

Look into Baumgarte Stabilization for stacked object impulses. Not gonna work, need to use position correction

Make sure core solver functions vectorize.

Figure out how rotations work.

