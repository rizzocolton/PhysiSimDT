#ifndef SPATIALHASHMAP_H
#define SPATIALHASHMAP_H

#include <vector>
#include <cstdint>

class SpatialHashMap{
    private:
        int cellTotal;
        int entityTotal;
        float cellSize;

        std::vector<int> entityHashes; //stores the hash of each entity's current cell
        std::vector<int> cellCounts; //stores the number of entities in each cell, used for quick clearing of cells when they are updated
        std::vector<int> cellOffsets; //stores the starting index of each cell in the entity list, used for quick access to entities in each cell
        std::vector<int> sortedEntities; //stores the indices of entities sorted by their cell hash, used for efficient iteration over entities in each cell

        int getHash(float x, float y) const;
    
    public:
        //default constructor, does nothing, used for default initialization of SpatialHashMap in PhysicsState, must be properly initialized with the parameterized constructor before use
        SpatialHashMap() = default;
        //constructor takes the max number of entities, the size of the map, and the size of each cell
        SpatialHashMap(int maxEntities, int sizeX, int sizeY, float cellSize);
        //updates every frame, rebuilds the hasmap based off the current positions of all entities
        void build(const std::vector<float>& x, const std::vector<float>& y);

};

#endif