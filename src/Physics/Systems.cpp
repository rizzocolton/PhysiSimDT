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

void Systems::MovementIndividual(PhysicsState& state, int i, float dt){
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

void Systems::BoundaryCollisions(PhysicsState& state, float dt, float restitution){

    for(int i=0;i<state.hasRadius.size();i++){
        int pId=state.hasRadius[i];
        if(state.radius[pId]+state.x[pId]>state.maxx){
            //out of right wall
            
            //first calculate time in wall
            float tInWall=abs((state.radius[pId]+state.x[pId]-state.maxx)/(state.vx[pId]));

            //then push object back to where it collided
            state.x[pId]-=state.vx[pId]*tInWall;
            state.y[pId]-=state.vy[pId]*tInWall;

            //figure out time remaining in frame
            float tRemaining=dt-tInWall;

            //flip appropriate v component
            state.vx[pId]=-state.vx[pId];

            //progress remaining amount in frame
            MovementIndividual(state,pId,tRemaining);

        }else if(state.radius[pId]>state.x[pId]){
            //out of left wall, similar logic follows
            float tInWall=abs((state.radius[pId]-state.x[pId])/(state.vx[pId]));
            state.x[pId]-=state.vx[pId]*tInWall;
            state.y[pId]-=state.vy[pId]*tInWall;
            float tRemaining=dt-tInWall;
            state.vx[pId]=-state.vx[pId];
            MovementIndividual(state,pId,tRemaining);
        }
        if(state.radius[pId]+state.y[pId]>state.maxy){
            //out of top wall, similar logic follows
            float tInWall=abs((state.radius[pId]+state.y[pId]-state.maxy)/(state.vy[pId]));
            state.x[pId]-=state.vx[pId]*tInWall;
            state.y[pId]-=state.vy[pId]*tInWall;
            float tRemaining=dt-tInWall;
            state.vy[pId]=-state.vy[pId];
            MovementIndividual(state,pId,tRemaining);

        }else if(state.radius[pId]>state.y[pId]){
            //out of bottom wall, similar logic follows
            float tInWall=abs((state.radius[pId]-state.y[pId])/(state.vy[pId]));
            state.x[pId]-=state.vx[pId]*tInWall;
            state.y[pId]-=state.vy[pId]*tInWall;
            float tRemaining=dt-tInWall;
            state.vy[pId]=-state.vy[pId];
            MovementIndividual(state,pId,tRemaining);
        }
    }

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

                //if circles are moving away from eachother, move on
                if(vnormal<0.f) continue;
                
                //correct each circles position weighted by their mass, mass variable will be useful for impulse as well
                float totalInvInvMass=1.f/(state.invmass[id1]+state.invmass[id2]);

                //the correction should be diff-radius so we need to get radius vector
                Vector2f r=radiusSum*n;
                diff=diff-r;
                float correctionPercentage1=totalInvInvMass*state.invmass[id2];
                float correctionPercentage2=totalInvInvMass*state.invmass[id1];
                state.x[id1]-=diff.x/2.f*correctionPercentage1;
                state.y[id1]-=diff.y/2.f*correctionPercentage1;
                state.x[id2]+=diff.x/2.f*correctionPercentage2;
                state.y[id2]+=diff.y/2.f*correctionPercentage2;
            
                //finding impulse magnitude
                float J=-(1+restitution)*vnormal;
                J*=totalInvInvMass;

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