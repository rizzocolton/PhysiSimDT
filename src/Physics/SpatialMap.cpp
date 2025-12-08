#include "SpatialMap.h"
#include "Objects/Circle.h"
#include "Constants.h"

SpatialMap::SpatialMap(int cs){
    cellSize=cs;
    for(int i=0;i<SCREEN_WIDTH;i+=cellSize){
        for(int j=0;j<SCREEN_HEIGHT;j+=cellSize){
            sf::RectangleShape cell;
            cell.setPosition({static_cast<float>(i),static_cast<float>(j)});
            cell.setSize({static_cast<float>(cellSize),static_cast<float>(cellSize)});
            cell.setFillColor(sf::Color::Transparent);
            cell.setOutlineThickness(0.5f);
            cells.push_back(cell);
        }
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
    for(sf::RectangleShape& cell:cells){
        window.draw(cell);
    }
}