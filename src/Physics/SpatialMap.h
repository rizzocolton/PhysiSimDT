#ifndef SPATIALMAP_H
#define SPATIALMAP_H

#include <SFML/Graphics.hpp>
#include "GridKey.h"
#include<unordered_map>
#include<unordered_set>
#include<vector>

//Forward declaration of circle to avoid circular dependency
class Circle;




class SpatialMap{
private:
    //hashmap with key representing a cell and value representing the circles contained in the map
    //need to use custom hash for the custome gridkey struct
    std::unordered_map<GridKey,std::unordered_set<Circle*>,GridKeyHash> sm;
    int cellSize;
    sf::VertexArray grid;
    

public:
    //Constructor

    SpatialMap(int cs, sf::FloatRect bounds);

    GridKey getKey(Circle& c);

    //alternate constructor for mouse button checks
    GridKey getKey(sf::Vector2f p);
    
    void enterCell(Circle* c);

    int getCellSize();

    void clear();

    std::unordered_map<GridKey,std::unordered_set<Circle*>,GridKeyHash>& getMap();

    //Display function

    void draw(sf::RenderWindow& window);

};


#endif