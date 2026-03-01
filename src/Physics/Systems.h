#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "PhysicsState.h"

struct Systems{

    //Makes all force States zero to prepare for new frame
    static void ZeroForces(PhysicsState& state);
    //Applies global gravity
    static void GlobalGravity(PhysicsState& state, float gravity);
    //THE numerical integration step. Uses integration techniques to move objects based on force and velocity. Currently uses Verlet
    static void Movement(PhysicsState& state, float dt);
    //Checks for collisions with boundaries and applies appropriate response (velocity reversal and restitution)
    static void BoundaryCollisions(PhysicsState& state, float dt, float restitution);
    //Checks for collisions between objects and applies appropriate response based on restitution
    static void Collisions(PhysicsState& state, float dt, float restitution);


    //helpful state reading functions
    static float getKineticEnergy(PhysicsState& state);
    static float getGravitationalPotentialEnergySimple(PhysicsState& state, float gravity);
};

//Vector struct to make physics calculations more readable

struct Vector2f{
    float x,y;

    float magSq(){
        return x*x+y*y;
    };

    float mag(){
        return sqrt(x*x+y*y);
    }

    //returns a unit vector of this vector
    Vector2f normalize(){
        float mag=this->mag();
        return {x/mag,y/mag};
    }

    //returns dot product of this vector with another
    float dot(Vector2f other){
        return(x*other.x+y*other.y);
    }
};

//vector addition
inline Vector2f operator+(const Vector2f left, const Vector2f right){
    return {left.x+right.x,left.y+right.y};
};

//vector subtraction
inline Vector2f operator-(const Vector2f left, const Vector2f right){
    return {left.x-right.x,left.y-right.y};
}

//vector scalar mulitplication
inline Vector2f operator*(const float left, const Vector2f right){
    return{left*right.x,left*right.y};
}

#endif