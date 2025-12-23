#include "SpatialMap.h"
#include "Objects/Circle.h"

SpatialMap::SpatialMap(int cs, sf::FloatRect bounds){
    cellSize=cs;
    this->bounds=bounds;
    grid.setPrimitiveType(sf::PrimitiveType::Lines);
    //need to resize for 2 vertices for every line, should be bounds/cellsize lines for each axis
    grid.resize(2*(static_cast<int>(bounds.size.y)+static_cast<int>(bounds.size.x))/cellSize);
    int i=0;
    for(int x=bounds.position.x;x<bounds.size.x+bounds.position.x;x+=cellSize){
        grid[i].position=sf::Vector2f{static_cast<float>(x),bounds.position.y};
        i++;
        grid[i].position=sf::Vector2f{static_cast<float>(x),bounds.size.y+bounds.position.y};
        i++;
    }
    for(int y=bounds.position.y;y<bounds.size.y+bounds.position.y;y+=cellSize){
        grid[i].position=sf::Vector2f{bounds.position.x,static_cast<float>(y)};
        i++;
        grid[i].position=sf::Vector2f{bounds.size.x+bounds.position.x,static_cast<float>(y)};
        i++;
    }
}


GridKey SpatialMap::getKey(PhysicsObject& c){
    return GridKey(c.getPos(),cellSize);
}

GridKey SpatialMap::getKey(sf::Vector2f p){
    return GridKey(p,cellSize);
}

void SpatialMap::enterCell(PhysicsObject* c){
    sm[getKey(*c)].insert(c);
}

int SpatialMap::getCellSize(){
    return cellSize;
}

void SpatialMap::setCellSize(int cs){
    cellSize=cs;
    grid.clear();

    for(int x = static_cast<int>(bounds.position.x);
        x < static_cast<int>(bounds.position.x + bounds.size.x);
        x += cellSize)
    {
        sf::Vertex v1;
        v1.position = sf::Vector2f(static_cast<float>(x), bounds.position.y);
        v1.color = sf::Color::White;
        grid.append(v1);

        sf::Vertex v2;
        v2.position = sf::Vector2f(static_cast<float>(x), bounds.position.y + bounds.size.y);
        v2.color = sf::Color::White;
        grid.append(v2);
    }

    for(int y = static_cast<int>(bounds.position.y);
        y < static_cast<int>(bounds.position.y + bounds.size.y);
        y += cellSize)
    {
        sf::Vertex v1;
        v1.position = sf::Vector2f(bounds.position.x, static_cast<float>(y));
        v1.color = sf::Color::White;
        grid.append(v1);

        sf::Vertex v2;
        v2.position = sf::Vector2f(bounds.position.x + bounds.size.x, static_cast<float>(y));
        v2.color = sf::Color::White;
        grid.append(v2);
    };
}

void SpatialMap::clear(){
    sm.clear();
}

std::unordered_map<GridKey,std::unordered_set<PhysicsObject*>,GridKeyHash>& SpatialMap::getMap(){
    return sm;
}

void SpatialMap::draw(sf::RenderWindow& window){
    window.draw(grid);
}