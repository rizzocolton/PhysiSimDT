#ifndef SPATIALMAP_H
#define SPATIALMAP_H

#include <SFML/Graphics.hpp>
#include "GridKey.h"
#include<unordered_map>
#include<unordered_set>
#include<vector>

//Forward declaration of physics object to avoid circular dependency
class PhysicsObject;




class SpatialMap{
private:
    //hashmap with key representing a cell and value representing the physics objects contained in the map
    //need to use custom hash for the custome gridkey struct
    std::unordered_map<GridKey,std::unordered_set<PhysicsObject*>,GridKeyHash> sm;
    int cellSize;
    sf::VertexArray grid;
    sf::FloatRect bounds;
    

public:
    //Constructor

    SpatialMap(int cs, sf::FloatRect bounds);

    GridKey getKey(PhysicsObject& c);

    //alternate constructor for mouse button checks
    GridKey getKey(sf::Vector2f p);
    
    void enterCell(PhysicsObject* c,float scaleFactor);

    int getCellSize();

    void setCellSize(int cs);

    void clear();

    std::unordered_map<GridKey,std::unordered_set<PhysicsObject*>,GridKeyHash>& getMap();

    //Display function

    void draw(sf::RenderWindow& window);

};


#endif