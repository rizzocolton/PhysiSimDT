#ifndef SPATIALMAP_H
#define SPATIALMAP_H

#include <SFML/Graphics.hpp>
#include "Circle.h"
#include<unordered_map>
#include<unordered_set>
#include<vector>

struct GridKey{
    int x;
    int y;

    GridKey(sf::Vector2f p){
        x=static_cast<int>(p.x);
        y=static_cast<int>(p.y);
    }

    //found this online, needed to make sure that equalities work between GridKeys
    bool operator==(const GridKey& other) const{
        return x==other.x&&y==other.y;
    }
};

struct GridKeyHash{
    //also found this online, a simple hash depending on x and y
    std::size_t operator()(const GridKey& gk) const{
        //hashes each component
        std::size_t h1=std::hash<int>{}(gk.x);
        std::size_t h2=std::hash<int>{}(gk.y);
        
        //then combines components
        return h1^(h2<<1);
    }
};

class SpatialMap{
private:
    //hashmap with key representing a cell and value representing the circles contained in the map
    //need to use custom hash for the custome gridkey struct
    std::unordered_map<GridKey,std::unordered_set<Circle*>,GridKeyHash> sm;
    int cellSize;
    std::vector<sf::RectangleShape> cells;
    

public:
    //Constructor

    SpatialMap(int cs);

    GridKey getKey(Circle& c);
    
    void enterCell(Circle* c);

    void exitCell(Circle* c);

    //Display function

    std::unordered_map<GridKey,std::unordered_set<Circle*>,GridKeyHash> getMap();

    void draw(sf::RenderWindow& window);

};


#endif