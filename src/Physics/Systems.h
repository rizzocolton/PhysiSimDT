#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "PhysicsState.h"

struct Systems{
    //Makes all force States zero to prepare for new frame
    static void ZeroForces(PhysicsState& state);
    //Applies global gravity
    static void GlobalGravity(PhysicsState& state, float gravity);
    //THE numerical integration step. Uses integration techniques to move objects based on force and velocity. Currently uses simple Euler integration, but can be changed to more complex methods like Verlet or RK4.
    static void Movement(PhysicsState& state, float dt);
    //Checks for collisions with boundaries and applies appropriate response (velocity reversal and restitution)
    static void BoundaryCollisions(PhysicsState& state, float restitution);
    //Checks for collisions between objects and applies appropriate response based on restitution
    static void Collisions(PhysicsState& state, float restitution);
};


#endif