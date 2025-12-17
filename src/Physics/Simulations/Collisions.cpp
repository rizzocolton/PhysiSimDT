#include "Collisions.h"

Collisions::Collisions(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds)
: gravity(gravity), colRestitution(colRestitution), boundsRestitution(boundsRestitution), sm(cellSize,bounds), simBounds(bounds){
    //Initialize with some circles for testing
    for(int i=0;i<10000;i++){
        sf::Vector2f position{static_cast<float>(rand()%1400+500),static_cast<float>(rand()%800)};
        float radius=1.f;
        float mass=radius*radius*3.14f; //mass proportional to area
        sf::Color color(
            (rand()%256),
            (rand()%256),
            (rand()%256)
        );
        objects.push_back(std::make_unique<Circle>(position, radius, mass, color));
    }
}

void Collisions::update(float dt){
    //Clear spatial map for new frame
    sm.clear();
    //Update position of all objects
    for(auto& obj : objects){
        sf::Vector2f weight{0.0f,obj->getMass()*gravity};
        obj->push(weight,dt);
        obj->update(dt);
        obj->checkBounds(simBounds,boundsRestitution);
        sm.enterCell(obj.get());
    }

    //Check for collisions between objects in cells and adjacent cells
    for(auto& [cell,objs]: sm.getMap()){
        //makes a vector of all objs in set
        std::vector<PhysicsObject*> objVec(objs.begin(),objs.end());
        for(int i=0;i<objVec.size();i++){
            for(int j=i+1;j<objVec.size();j++){
                objVec[i]->collide(*objVec[j],colRestitution);
            }
        }

        //check adjacent cells
        for(int i=0;i<objVec.size();i++){
            //right cell
            GridKey rightKey(cell.getX()+1,cell.getY());
            if(sm.getMap().count(rightKey)!=0){
                for(auto& otherObj : sm.getMap().at(rightKey)){
                    objVec[i]->collide(*otherObj,colRestitution);
                }
            }

            //bottom cell
            GridKey bottomKey(cell.getX(),cell.getY()+1);
            if(sm.getMap().count(bottomKey)!=0){
                for(auto& otherObj : sm.getMap().at(bottomKey)){
                    objVec[i]->collide(*otherObj,colRestitution);
                }
            }

            //bottom-right cell
            GridKey bottomRightKey(cell.getX()+1,cell.getY()+1);
            if(sm.getMap().count(bottomRightKey)!=0){
                for(auto& otherObj : sm.getMap().at(bottomRightKey)){
                    objVec[i]->collide(*otherObj,colRestitution);
                }
            }

            //bottom-left cell
            GridKey bottomLeftKey(cell.getX()-1,cell.getY()+1);
            if(sm.getMap().count(bottomLeftKey)!=0){
                for(auto& otherObj : sm.getMap().at(bottomLeftKey)){
                    objVec[i]->collide(*otherObj,colRestitution);
                }
            }
        }
    }
}

void Collisions::draw(sf::RenderWindow& window){
    //Optionally draw spatial map grid
    //sm.draw(window);

    //Draw all objects
    for(auto& obj : objects){
        obj->draw(window);
    }
}

void Collisions::initUI(){
    //Placeholder for future UI elements

}

void Collisions::drawUI(sf::RenderWindow& window){
    //Placeholder for future UI elements
}

void Collisions::handleEvent(const sf::Event& event){
    
}

int Collisions::getPopulation(){
    return static_cast<int>(objects.size());
}

