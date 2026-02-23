#ifndef PHYSICSSTATE_H
#define PHYSICSSTATE_H

#include SpatialHashMap.h

class PhysicsState{
    public:
        //CORE KINEMATICS ALL OBJECTS MUST HAVE

        std::vector<float> x,y; //position
        std::vector<float> vx,vy; //velocity
        std::vector<float> ax,ay; //acceleration
        std::vector<float> fx,fy; //force
        std::vector<float> invmass; //mass inverse (for easier calculations, as multiplying by inverse is faster than dividing by mass)
        
        //ARBITRARY COMPONENTS

        //std::vector<float> radius; //for collision detection with circles
        //std::vector<float> charge; //for EM sim

        //GLOBAL STRUCTURES
        SpatialHashMap sm; //spatial map for efficient collision detection
};

#endif