#ifndef SPATIALHASH_H
#define SPATIALHASH_H

#include <vector>
struct AABB{
    float xMin,xMax;
    float yMin,yMax;
};


class SpatialHash{
private:
    float conversionFactor;
    int width;
    int height;

    //Parallel hash tables in a linked list esc format 
    //if index in cellHead is -1, no entity exists there, if not it has the id of an entity
    std::vector<int> cellHead;
    //that id of an entity as an index in this vector will give the next id in that cell 
    std::vector<int> nextEntity;
    //you can follow the train until you hit a -1

    inline int getHash(float x, float y) const{
        int bucketX=static_cast<int>(x*conversionFactor);
        int bucketY=static_cast<int>(y*conversionFactor);
        return bucketX+bucketY*width;
    }  

public:
    SpatialHash(float cellSize, int width, int height, int maxEntities);

    //this is so the compiler doesn't yell at me
    SpatialHash()=default;

    void clear();

    void insert(int particleId, const AABB& bounds);

};

#endif