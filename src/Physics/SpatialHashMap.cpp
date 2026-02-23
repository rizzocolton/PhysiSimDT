#include "SpatialHashMap.h"

SpatialHashMap::SpatialHashMap(int maxEntities, int sizeX, int sizeY, float cellSize):
    entityTotal(maxEntities), cellSize(cellSize){
    //calculate total number of cells based on map size and cell size, then resize vectors accordingly
    cellTotal=sizeX*sizeY/(cellSize*cellSize);
    entityHashes.resize(maxEntities);
    cellCounts.resize(cellTotal);
    cellOffsets.resize(cellTotal+1);
    sortedEntities.resize(maxEntities);
}

int SpatialHashMap::getHash(float x, float y) const{
    //calculate cell coordinates
    int cellX=static_cast<int>(x/cellSize);
    int cellY=static_cast<int>(y/cellSize);

    //combine cell coordinates into a single hash value
    //took this hashing method from gemini, gives a valid address in cellCounts array
    uint32_t h = (cellX * 73856093) ^ (cellY * 19349663);
    return h % cellTotal;
}

void SpatialHashMap::build(const std::vector<float>& x, const std::vector<float>& y){
    //makes all cells empty
    std::fill(cellCounts.begin(), cellCounts.end(), 0);

    //goes through every entity, calculates its hash, and counts how many entities are in each cell
    for(int i=0; i<entityTotal; i++){
        int hash=getHash(x[i],y[i]);
        entityHashes[i]=hash;
        cellCounts[hash]++;
    }

    //calculates the starting index of each cell in the sortedEntities array using the cellCounts
    int currentOffset=0;
    for(int i=0;i<cellTotal;i++){
        cellOffsets[i]=currentOffset;
        currentOffset+=cellCounts[i];
    }
    //stores the total size at the end to safely fetch last cell
    cellOffsets[cellTotal]=currentOffset;

    //keeps track of where to insert the next entity in each cell
    std::vector<int> currentInserts=cellOffsets; 
    //goes through every entity again and inserts it into the sortedEntities array based on its hash and the current insert index for that cell
    for(int i=0; i<entityTotal; i++){
        int hash=entityHashes[i];
        int insertIndex=currentInserts[hash]++;
        //stores the entity id at the insert index in the sortedEntities array
        sortedEntities[insertIndex]=i;
    }
}
