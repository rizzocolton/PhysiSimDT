#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "PhysicsState.h"

class Systems{
    //Makes all force States zero to prepare for new frame
    static void ZeroForces(PhysicsState& reg);
    //Applies global gravity
    static void GlobalGravity(PhysicsState& reg, float gravity);
    //THE numerical integration step. Uses integration techniques to move objects based on force and velocity. Currently uses simple Euler integration, but can be changed to more complex methods like Verlet or RK4.
    static void Movement(PhysicsState& reg, float dt);
    //Checks for collisions with boundaries and applies appropriate response (velocity reversal and restitution)
    static void BoundaryCollision(PhysicsState& reg, float restitution);
    //Checks for collisions between objects and applies appropriate response based on restitution
    static void Collisions(PhysicsState& reg, float restitution);
};


#endif