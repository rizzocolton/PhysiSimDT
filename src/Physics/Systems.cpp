#include "Systems.h"
#include <execution>
#include <iostream>

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

    //Circle boundary collisions, can use parallelization since there are no race conditions here
    std::for_each(std::execution::par_unseq, state.hasRadius.begin(), state.hasRadius.end(), [&](int particleId){
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

void Systems::Collisions(PhysicsState& state, float dt, float restitution){
    for(int i=0; i<state.hasRadius.size();i++){
        int id1=state.hasRadius[i];
        for(int j=i+1; j<state.hasRadius.size();j++){
            int id2=state.hasRadius[j];
            
            Vector2f p1{state.x[id1],state.y[id1]};
            Vector2f p2{state.x[id2],state.y[id2]};
            Vector2f diff=p2-p1;
            float distSq=diff.magSq();
            float radiusSum=state.radius[i]+state.radius[j];

            if(distSq<radiusSum*radiusSum){ //if the distance between the centers of the circles is less than the sum of their radii, they are colliding
                //helpful vector assignment of the relative velocity
                Vector2f rv{state.vx[id1]-state.vx[id2],state.vy[id1]-state.vy[id2]};

                //finding the unit normal coming pointing from circle one to two
                Vector2f n=diff.normalize();

                //finding v normal
                float vnormal=rv.dot(n);
                //The baumgarte correction
                Vector2f overlap=radiusSum*n-diff;
                float correction=BAUMGARTE_BETA*overlap.dot(n)/dt;
            
                //finding impulse magnitude
                float J=-(1+restitution)*vnormal-correction;
                J/=(state.invmass[id1]+state.invmass[id2]);

                //adding impulse to original velocities in state
                state.vx[id1]+=J*n.x*state.invmass[id1];
                state.vx[id2]-=J*n.x*state.invmass[id2];
                state.vy[id1]+=J*n.y*state.invmass[id1];
                state.vy[id2]-=J*n.y*state.invmass[id2];
            }
        }
    }
}

float Systems::getKineticEnergy(PhysicsState& state){
    float KE=0.f;
    for(int i=0;i<state.population;i++){
        if(state.invmass[i]==0.f) continue;
        KE+=.5f*(state.vx[i]*state.vx[i]+state.vy[i]*state.vy[i])/state.invmass[i];
    }
    return KE;
}

float Systems::getGravitationalPotentialEnergySimple(PhysicsState& state, float gravity){
    float U=0.f;
    for(int i=0;i<state.population;i++){
        if(state.invmass[i]==0.f) continue;
        U+=-gravity*state.y[i]/state.invmass[i];
    }
    return U;
}