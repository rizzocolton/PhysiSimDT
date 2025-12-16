#include "Collisions.h"

Collisions::Collisions(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds)
: gravity(gravity), colRestitution(colRestitution), boundsRestitution(boundsRestitution), sm(cellSize,bounds), simBounds(bounds){
    //Initialize with some circles for testing
    for(int i=0;i<50;i++){
        sf::Vector2f position{static_cast<float>(rand()%1400+500),static_cast<float>(rand()%800)};
        float radius=10.f+static_cast<float>(rand()%20);
        float mass=radius*radius*3.14f; //mass proportional to area
        sf::Color color(
            (rand()%256),
            (rand()%256),
            (rand()%256)
        );
        objects.emplace_back(position,radius,mass,color);
    }
}

void Collisions::update(float dt){
    //Clear spatial map for new frame
    sm.clear();
    //Update position of all objects
    for(auto& obj : objects){
        sf::Vector2f weight{0.0f,obj.getMass()*gravity};
        obj.push(weight,dt);
        obj.update(dt);
        obj.checkBounds(simBounds,boundsRestitution);
        sm.enterCell(&obj);
    }

    //Check for collisions between objects in cells and adjacent cells
    for(auto& [cell,objs]: sm.getMap()){
        //makes a vector of all objs in set
        std::vector<Circle*> objVec(objs.begin(),objs.end());
        for(int i=0;i<objVec.size();i++){
            GridKey currentKey=sm.getKey(*objVec[i]);
            int cellSize=sm.getCellSize();
            sf::Vector2f objPos=objVec[i]->getPos();
    
            // Check 3x3 grid 
            for(int dx=-1;dx<=1;dx++){
                for(int dy=-1;dy<=1;dy++){
                    sf::Vector2f adjPos(
                        objPos.x+dx*cellSize, 
                        objPos.y+dy*cellSize
                    );
                    GridKey adjKey(adjPos,cellSize);
            
                    auto& map=sm.getMap();
                    if(map.count(adjKey)!=0){
                        for(Circle* otherObj : map[adjKey]){
                            objVec[i]->collide(*otherObj,colRestitution);
                        }
                    }
                }
            }
        }
    }
    
}

void Collisions::draw(sf::RenderWindow& window){
    //Optionally draw spatial map grid
    sm.draw(window);

    //Draw all objects
    for(auto& obj : objects){
        obj.draw(window);
    }
}

void Collisions::drawUI(sf::RenderWindow& window){
    //Placeholder for future UI elements
}

void Collisions::handleEvent(const sf::Event& event){
    
}

