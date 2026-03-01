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

            //then reset its velocity to what it was
            state.vx[pId]-=state.ax[pId]*tInWall;
            state.vy[pId]-=state.ay[pId]*tInWall;

            //flip appropriate v component
            state.vx[pId]=-state.vx[pId];

        }else if(state.radius[pId]>state.x[pId]){
            //out of left wall, similar logic follows
            float tInWall=abs((state.radius[pId]-state.x[pId])/(state.vx[pId]));
            state.x[pId]-=state.vx[pId]*tInWall;
            state.y[pId]-=state.vy[pId]*tInWall;
            state.vx[pId]-=state.ax[pId]*tInWall;
            state.vy[pId]-=state.ay[pId]*tInWall;
            state.vx[pId]=-state.vx[pId];
        }
        if(state.radius[pId]+state.y[pId]>state.maxy){
            //out of top wall, similar logic follows
            float tInWall=abs((state.radius[pId]+state.y[pId]-state.maxy)/(state.vy[pId]));
            state.x[pId]-=state.vx[pId]*tInWall;
            state.y[pId]-=state.vy[pId]*tInWall;
            state.vx[pId]-=state.ax[pId]*tInWall;
            state.vy[pId]-=state.ay[pId]*tInWall;
            state.vy[pId]=-state.vy[pId];

        }else if(state.radius[pId]>state.y[pId]){
            //out of bottom wall, similar logic follows
            float tInWall=abs((state.radius[pId]-state.y[pId])/(state.vy[pId]));
            state.x[pId]-=state.vx[pId]*tInWall;
            state.y[pId]-=state.vy[pId]*tInWall;
            state.vx[pId]-=state.ax[pId]*tInWall;
            state.vy[pId]-=state.ay[pId]*tInWall;
            state.vy[pId]=-state.vy[pId];
        }
    }

}

void Systems::Collisions(PhysicsState& state, float dt, float restitution){
    //FIRST SPATIAL HASH NEEDS CLEARED AND REPOPULATED (just for circles for now)
    state.sh.clear();
    for(int i=0; i<state.hasRadius.size();i++){
        int circleId=state.hasRadius[i];
        state.sh.insert(circleId,state.x[circleId],state.y[circleId]);
    }

    //THEN NARROW PHASE COLLISION IS DEFINED
    auto checkAndResolve=[&](int id1, int id2){
        Vector2f p1{state.x[id1],state.y[id1]};
        Vector2f p2{state.x[id2],state.y[id2]};
        Vector2f diff=p2-p1;
        float distSq=diff.magSq();
        float radiusSum=state.radius[id1]+state.radius[id2];
        if(distSq<radiusSum*radiusSum){ //if the distance between the centers of the circles is less than the sum of their radii, they are colliding
            //helpful vector assignment of the relative velocity
            Vector2f rv{state.vx[id1]-state.vx[id2],state.vy[id1]-state.vy[id2]};
            //finding the unit normal coming pointing from circle one to two
            Vector2f n=diff.normalize();
            //finding v normal
            float vnormal=rv.dot(n);
            
            //correct each circles position weighted by their mass, mass variable will be useful for impulse as well
            float totalInvInvMass=1.f/(state.invmass[id1]+state.invmass[id2]);
            //the correction should be radius-diff so we need to get radius vector
            Vector2f r=radiusSum*n;
            diff=r-diff;
            float correctionPercentage1=totalInvInvMass*state.invmass[id1];
            float correctionPercentage2=totalInvInvMass*state.invmass[id2];
            state.x[id1]-=diff.x*correctionPercentage1;
            state.y[id1]-=diff.y*correctionPercentage1;
            state.x[id2]+=diff.x*correctionPercentage2;
            state.y[id2]+=diff.y*correctionPercentage2;
            //if circles are moving away from eachother, move on before calculating impulse
            if(vnormal<0.f){
                id2=state.sh.nextEntity[id2];
                return;
            } 
        
            //finding impulse magnitude
            float J=-(1+restitution)*vnormal;
            J*=totalInvInvMass;
            //adding impulse to original velocities in state
            state.vx[id1]+=J*n.x*state.invmass[id1];
            state.vx[id2]-=J*n.x*state.invmass[id2];
            state.vy[id1]+=J*n.y*state.invmass[id1];
            state.vy[id2]-=J*n.y*state.invmass[id2];
        }
    };

    //NOW DO BROAD PHASE COLLISION DETECTION
    int gridWidth = state.sh.width;
    int gridHeight = state.sh.height;

    for(int y=0;y<gridHeight;y++){
        for(int x=0;x<gridWidth;x++){
            int cellIndex=x+(y*gridWidth);
            int id1 = state.sh.cellHead[cellIndex];
            
            while(id1 != -1){
                // A. Check against own cell
                int id2 = state.sh.nextEntity[id1];
                while(id2 != -1){
                    checkAndResolve(id1, id2);
                    id2 = state.sh.nextEntity[id2];
                }

                // B. Check against objects in 4 neighboring cells
                // Right, Bottom-Left, Bottom, Bottom-Right
                int neighborOffsets[4][2] = {{1,0}, {-1,1}, {0,1}, {1,1}}; 
                
                for(int i = 0; i < 4; i++){
                    int nx = x + neighborOffsets[i][0];
                    int ny = y + neighborOffsets[i][1];
                    
                    if(nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight){
                        int neighborId = state.sh.cellHead[nx + (ny * gridWidth)];
                        
                        while(neighborId != -1){
                            checkAndResolve(id1, neighborId);
                            neighborId = state.sh.nextEntity[neighborId];
                        }
                    }
                }
                
                id1 = state.sh.nextEntity[id1];
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