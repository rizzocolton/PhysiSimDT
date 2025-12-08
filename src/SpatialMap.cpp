#include "SpatialMap.h"
#include "Constants.h"

SpatialMap::SpatialMap(int cs){
    cellSize=cs;
    for(int i=0;i<SCREEN_WIDTH;i+=cellSize){
        for(int j=0;j<SCREEN_HEIGHT;j+=cellSize){
            sf::RectangleShape cell;
            cell.setPosition({static_cast<float>(i),static_cast<float>(j)});
            cell.setSize({static_cast<float>(cellSize),static_cast<float>(cellSize)});
            cell.setFillColor(sf::Color::Transparent);
            cell.setOutlineThickness(1.f);
            cells.push_back(cell);
        }
    }
}


GridKey SpatialMap::getKey(Circle& c){
    return GridKey(c.getPos());
}

void SpatialMap::enterCell(Circle* c){
    GridKey current=getKey(*c.getPos());
    if(sm.count(current)==0){
        std::unordered_set newSet={c};
        sm.insert({current,newSet});
    }else if(sm.at(current).count(c)==0){
        std::unordered_set currentSet=sm.at(current);
        sm.erase(current);
        currentSet.insert(c);
        sm.insert({current,currentSet});
    }
}

void SpatialMap::exitCell(Circle* c){

}

std::unordered_map<GridKey,std::unordered_set<Circle*>,GridKeyHash> SpatialMap::getMap(){
    return sm;
}

void SpatialMap::draw(sf::RenderWindow& window){
    for(sf::RectangleShape& cell:cells){
        window.draw(cell);
    }
}