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
    std::vector<float> radius; //for collision detection with circles
    std::vector<float> charge; //for EM sim

    //GLOBAL STRUCTURES AND VARIABLES
    float maxy; //maximum y value in the simulation, used for spatial map bounds and boundary checks
    float maxx; //maximum x value in the simulation, used for spatial map bounds and boundary checks
    int population=0; //number of entities currently in the simulation, used for iterating over entities and spawning new ones
    int maxPopulation; //maximum number of entities allowed in the simulation, used to prevent spawning more entities than the vectors can handle

    SpatialHashMap sm; //spatial map for efficient collision detection


    //HELPER FUNCTIONS
    
    //basically an instantiation, just reserves space in all vectors for the max number of entities, and initializes the spatial map with the appropriate parameters
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
        radius.reserve(maxEntities);
        charge.reserve(maxEntities);
        sm=SpatialHashMap(maxEntities,maxx,maxy,5.f); //initial cell size of 5 meters, can be changed later with sm.setCellSize()
        maxPopulation=maxEntities;
    }

    int spawnParticle(float x, float y){
        if(population>=maxPopulation){ //if we have reached max population, do not spawn more entities
            return -1;
        }
        this->x.push_back(x);
        this->y.push_back(y);
        vx.push_back(0.f);
        vy.push_back(0.f);
        ax.push_back(0.f);
        ay.push_back(0.f);
        fx.push_back(0.f);
        fy.push_back(0.f);
        invmass.push_back(1.f); //default mass of 1 kg
        population++;
        return population-1; //return the index of the newly spawned entity
    }
};

#endif