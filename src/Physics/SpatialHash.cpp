#include "SpatialHash.h"


SpatialHash::SpatialHash(float cellSize, int simWidth, int simHeight, int maxEntities):
     conversionFactor(1.f/cellSize),width(simWidth*conversionFactor+1),height(simHeight*conversionFactor+1){

        //reserving and filling appropriate space for each vector
        cellHead.resize(width*height,-1);
        nextEntity.resize(maxEntities,-1);
};

void SpatialHash::clear(){
    //just need to clear cellHead as its the only one accessed first during checks
    std::fill(cellHead.begin(),cellHead.end(),-1);
};

void SpatialHash::insert(int particleId, float x, float y){
    //get top right and bottom left indices
    int cell=getHash(x,y);
    //make next id be whatever cell head was referencing
    nextEntity[particleId]=cellHead[cell];
    //make the new cell head this ones id
    cellHead[cell]=particleId;
};