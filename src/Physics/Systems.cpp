#include "Systems.h"
#include <execution>

void Systems::ZeroForces(PhysicsState& state){
    std::fill(state.fx.begin(), state.fx.end(), 0.f);
    std::fill(state.fy.begin(), state.fy.end(), 0.f);
};

void Systems::GlobalGravity(PhysicsState& state, float gravity){
    for(int i=0; i<state.population; i++){
        if(state.invmass[i]>0){ //if the object has mass (invmass of 0 means infinite mass, so it should not be affected by forces)
            state.fy[i]+=gravity/state.invmass[i];
        }
    }
}

void Systems::Movement(PhysicsState& state, float dt){
    for(int i=0; i<state.population; i++){
        //numerical integration using velocity Verlet method

        //calculate velocity at half time step
        float vxHalf=state.vx[i]+0.5f*state.ax[i]*dt; 
        float vyHalf=state.vy[i]+0.5f*state.ay[i]*dt;

        //update position using velocity at half time step
        state.x[i]+=vxHalf*dt;
        state.y[i]+=vyHalf*dt;

        //update acceleration using new forces
        state.ax[i]=state.fx[i]*state.invmass[i];
        state.ay[i]=state.fy[i]*state.invmass[i];
        
        //update velocity using new acceleration
        state.vx[i]=vxHalf+0.5f*state.ax[i]*dt;
        state.vy[i]=vyHalf+0.5f*state.ay[i]*dt;
    }
}

void Systems::BoundaryCollisions(PhysicsState& state, float restitution){

    //Circle boundary collisions
    std::for_each(std::execution::par, state.hasRadius.begin(), state.hasRadius.end(), [&](int particleId){
        float radius = state.radius[particleId];
        if(state.x[particleId]<radius){
            state.x[particleId]=radius;
            if(state.vx[particleId]<0){ //only reverse velocity if moving towards the wall, otherwise we can get stuck objects with high restitution
                state.vx[particleId]=-state.vx[particleId]*restitution;
            }
        }else if(state.x[particleId]>state.maxx-radius){
            state.x[particleId]=state.maxx-radius;
            if(state.vx[particleId]>0){ //only reverse velocity if moving towards the wall, otherwise we can get stuck objects with high restitution
                state.vx[particleId]=-state.vx[particleId]*restitution;
            }
        }
        if(state.y[particleId]<radius){
            state.y[particleId]=radius;
            if(state.vy[particleId]<0){ //only reverse velocity if moving towards the wall, otherwise we can get stuck objects with high restitution
                state.vy[particleId]=-state.vy[particleId]*restitution;
            }
        }else if(state.y[particleId]>state.maxy-radius){
            state.y[particleId]=state.maxy-radius;
            if(state.vy[particleId]>0){ //only reverse velocity if moving towards the wall, otherwise we can get stuck objects with high restitution
                state.vy[particleId]=-state.vy[particleId]*restitution;
            }
        }
    });
}