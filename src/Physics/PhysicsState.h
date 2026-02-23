#ifndef PHYSICSSTATE_H
#define PHYSICSSTATE_H

#include "SpatialHashMap.h"

struct PhysicsState{
    //CORE KINEMATICS ALL OBJECTS MUST HAVE
    std::vector<float> x,y; //position
    std::vector<float> vx,vy; //velocity
    std::vector<float> ax,ay; //acceleration
    std::vector<float> fx,fy; //force
    std::vector<float> invmass; //mass inverse (for easier calculations, as multiplying by inverse is faster than dividing by mass)
    
    //ARBITRARY COMPONENTS
    //std::vector<float> radius; //for collision detection with circles
    //std::vector<float> charge; //for EM sim

    //GLOBAL STRUCTURES AND VARIABLES
    float maxy; //maximum y value in the simulation, used for spatial map bounds and boundary checks
    float maxx; //maximum x value in the simulation, used for spatial map bounds and boundary checks
    int population=0; //number of entities currently in the simulation, used to keep track of how many entities are actually being simulated (since vectors are reserved to max size but not necessarily filled)

    SpatialHashMap sm; //spatial map for efficient collision detection

    void reserve(int maxEntities){
        x.reserve(maxEntities);
        y.reserve(maxEntities);
        vx.reserve(maxEntities);
        vy.reserve(maxEntities);
        ax.reserve(maxEntities);
        ay.reserve(maxEntities);
        fx.reserve(maxEntities);
        fy.reserve(maxEntities);
        invmass.reserve(maxEntities);
        sm=SpatialHashMap(maxEntities,maxx,maxy,5.f); //initial cell size of 5 meters, can be changed later with sm.setCellSize()
    }
};

#endif