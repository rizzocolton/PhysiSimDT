#include "SpatialHash.h"


SpatialHash::SpatialHash(float cellSize, int width, int height, int maxEntities):
     conversionFactor(1.f/cellSize),width(width),height(height){

        //reserving and filling appropriate space for each vector
        cellHead.reserve(width*height);
        nextEntity.reserve(maxEntities);

        std::fill(cellHead.begin(),cellHead.end(),-1);
        std::fill(nextEntity.begin(),nextEntity.end(),-1);
};

void SpatialHash::clear(){
    //just need to clear cellHead as its the only one accessed first during checks
    std::fill(cellHead.begin(),cellHead.end(),-1);
};

void SpatialHash::insert(int particleId, const AABB& bounds){
    //get top right and bottom left indices
    int itr=getHash(bounds.xMax,bounds.yMax);
    //make next id be whatever cell head was referencing
    nextEntity[particleId]=cellHead[itr];
    //make the new cell head this ones id
    cellHead[itr]=particleId;

    int ibl=getHash(bounds.xMin,bounds.yMin);
    
    //if the bottom left and top right are in the same cell then we're done!
    if(ibl==itr) return;

    //otherwise we have to do this for every corner
    nextEntity[particleId]=cellHead[ibl];
    cellHead[ibl]=particleId;
    
    int itl=getHash(bounds.xMin,bounds.yMax);
    nextEntity[particleId]=cellHead[itl];
    cellHead[itl]=particleId;

    int ibr=getHash(bounds.xMax,bounds.yMin);
    nextEntity[particleId]=cellHead[ibr];
    cellHead[ibr]=particleId;

};