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