#include "SpatialMap.h"
#include "Objects/Circle.h"
#include "Constants.h"

SpatialMap::SpatialMap(int cs){
    cellSize=cs;
    grid.setPrimitiveType(sf::PrimitiveType::Lines);
    //need to resize for 2 vertices for every line, should be bounds/cellsize lines for each axis
    grid.resize(2*(static_cast<int>(SCREEN_HEIGHT)+static_cast<int>(SCREEN_WIDTH))/cellSize);
    int i=0;
    for(int x=0;x<SCREEN_WIDTH;x+=cellSize){
        grid[i].position=sf::Vector2f{static_cast<float>(x),0.f};
        i++;
        grid[i].position=sf::Vector2f{static_cast<float>(x),SCREEN_HEIGHT};
        i++;
    }
    for(int y=0;y<SCREEN_HEIGHT;y+=cellSize){
        grid[i].position=sf::Vector2f{0.f,static_cast<float>(y)};
        i++;
        grid[i].position=sf::Vector2f{SCREEN_WIDTH,static_cast<float>(y)};
        i++;
    }
}


GridKey SpatialMap::getKey(Circle& c){
    return GridKey(c.getPos(),cellSize);
}

void SpatialMap::enterCell(Circle* c){
    sm[getKey(*c)].insert(c);
}

int SpatialMap::getCellSize(){
    return cellSize;
}

void SpatialMap::clear(){
    sm.clear();
}

std::unordered_map<GridKey,std::unordered_set<Circle*>,GridKeyHash>& SpatialMap::getMap(){
    return sm;
}

void SpatialMap::draw(sf::RenderWindow& window){
    window.draw(grid);
}