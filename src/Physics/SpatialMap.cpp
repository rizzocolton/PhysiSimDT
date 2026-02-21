#include "SpatialMap.h"
#include "Objects/Circle.h"

SpatialMap::SpatialMap(int cs, sf::FloatRect bounds){
    cellSize=cs;
    this->bounds=bounds;
    grid.setPrimitiveType(sf::PrimitiveType::Lines);
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


GridKey SpatialMap::getKey(PhysicsObject& c){
    sf::Vector2f& pos = c.getPos();
    return GridKey(pos,cellSize);
}

GridKey SpatialMap::getKey(sf::Vector2f p){
    return GridKey(p,cellSize);
}

void SpatialMap::enterCell(PhysicsObject* c, float scaleFactor){
    sf::Vector2f pos = c->getPos();
    pos.x*=scaleFactor;
    pos.y*=scaleFactor;
    sm[getKey(pos)].insert(c);
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