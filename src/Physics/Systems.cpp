#include "Systems.h"

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
        //update velocity based on acceleration, then update position based on velocity, using simple Euler integration
        state.ax[i]=state.fx[i]*state.invmass[i];
        state.ay[i]=state.fy[i]*state.invmass[i];
        state.vx[i]+=state.ax[i]*dt;
        state.vy[i]+=state.ay[i]*dt;
        state.x[i]+=state.vx[i]*dt;
        state.y[i]+=state.vy[i]*dt;
    }
}

void Systems::BoundaryCollisions(PhysicsState& state, float restitution){
    for(int i=0; i<state.population; i++){
        //check for collision with left and right boundaries
        if(state.x[i]<0){
            state.x[i]=0;
            state.vx[i]=-state.vx[i]*restitution;
        }else if(state.x[i]>state.maxx){
            state.x[i]=state.maxx;
            state.vx[i]=-state.vx[i]*restitution;
        }
        //check for collision with top and bottom boundaries
        if(state.y[i]<0){
            state.y[i]=0;
            state.vy[i]=-state.vy[i]*restitution;
        }else if(state.y[i]>state.maxy){
            state.y[i]=state.maxy;
            state.vy[i]=-state.vy[i]*restitution;
        }
    }
}