#include "Collisions.h"
#include <iostream>

Collisions::Collisions(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds, std::function<void(SimType type)> func)
: gravity(gravity), colRestitution(colRestitution), sm(cellSize*scaleFactor,bounds), simBounds(bounds),boundsRestitution(boundsRestitution),switchSim(func){
    simBounds.position.x/=scaleFactor;
    simBounds.position.y/=scaleFactor;
    simBounds.size.x/=scaleFactor;
    simBounds.size.y/=scaleFactor;
}

void Collisions::update(float dt){
    //If simulation is not running, skip physics update
    if(!simulating){
        return;
    }

    //Clear spatial map for new frame
    sm.clear();

    //Update position of all objects
    for(auto& obj : objects){
        sf::Vector2f weight{0.0f,obj->getMass()*gravity};
        obj->move(dt);
        obj->checkBounds(simBounds,boundsRestitution);
        obj->push(weight,dt);
        //need scale factor for spatial map as it is in pixel units
        sm.enterCell(obj.get(),scaleFactor);
    }

    //Check for collisions between objects in cells and adjacent cells
    for(auto& [cell,objs]: sm.getMap()){
        //makes a vector of all objs in set
        std::vector<PhysicsObject*> objVec(objs.begin(),objs.end());
        for(int i=0;i<objVec.size();i++){
            for(int j=i+1;j<objVec.size();j++){
                objVec[i]->collide(*objVec[j],colRestitution);
            }

        //check adjacent cells
        
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

    timeElapsed+=dt; //add the amount of time elapsed in this frame
}

void Collisions::draw(sf::RenderWindow& window){
    //Optionally draw spatial map grid
    if(showGrid){
        sm.draw(window);
    }

    //Draw all objects
    for(auto& obj : objects){
        obj->draw(window,simBounds.position,scaleFactor);
    }
}

//initUI defined in SimInits under UI

void Collisions::drawUI(sf::RenderWindow& window){
    for(auto& element : UIElements){
        element->draw(window);
    }
}

void Collisions::handleEvent(const sf::Event& event){
    //translate raw mouse position into simulation coordinates (meters),
    //relative to the top‑left corner of the simBounds.
    sf::Vector2i raw = sf::Mouse::getPosition();
    sf::Vector2f mousePos;
    mousePos.x = (raw.x) / scaleFactor - simBounds.position.x;
    mousePos.y = (raw.y) / scaleFactor - simBounds.position.y;

    //if space pressed, toggle simulation state
    if(event.getIf<sf::Event::KeyPressed>()){
        if(event.getIf<sf::Event::KeyPressed>()->scancode==sf::Keyboard::Scancode::Space){
            simulating=!simulating;
        }
    }

    //if mouse is held down and moved, add circles along the drag path
    if(event.getIf<sf::Event::MouseMoved>()){
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            //if mouse is within simulation bounds, place a new circle at mouse position
            if(mousePos.x>0 && mousePos.x<simBounds.size.x &&
               mousePos.y>0 && mousePos.y<simBounds.size.y){
                sf::Vector2f position{mousePos};
                float radius=params.radius;
                float mass=params.mass;
                sf::Color color(
                    (rand()%256),
                    (rand()%256),
                    (rand()%256)
                );
                objects.push_back(std::make_unique<Circle>(position, radius, mass, color));
                objects.back()->setVel({params.vx,params.vy}); 
            }
        }
    }
    
    //if mouse pressed on sim screen, add a new circle at mouse position
    if(event.getIf<sf::Event::MouseButtonPressed>()){
        if(event.getIf<sf::Event::MouseButtonPressed>()->button==sf::Mouse::Button::Left){
            if(mousePos.x>0 && mousePos.x<simBounds.size.x &&
               mousePos.y>0 && mousePos.y<simBounds.size.y){
                sf::Vector2f position{mousePos};
                float radius=params.radius;
                float mass=params.mass;
                sf::Color color(
                    (rand()%256),
                    (rand()%256),
                    (rand()%256)
                );
                objects.push_back(std::make_unique<Circle>(position, radius, mass, color));
                objects.back()->setVel({params.vx,params.vy});
                sm.enterCell(objects.back().get(),scaleFactor);
            }
        }

        //if the right mouse is pressed, get the nearest object at that position
        if(event.getIf<sf::Event::MouseButtonPressed>()->button==sf::Mouse::Button::Right){
            //mouse's current position in pixels (spatial grid in pixels)
            sf::Vector2f mousePosf(mousePos.x*scaleFactor,mousePos.y*scaleFactor);
            //set of all objects at that cell
            std::unordered_set objs=sm.getMap()[sm.getKey(mousePosf)];
            //vector pointing to every object in that cell
            std::vector<PhysicsObject*> objVec;
            //filling that vector using extraction (C++17 exclusive thing)
            objVec.reserve(objs.size());
            while(!objs.empty()){
                objVec.push_back(std::move(objs.extract(objs.begin()).value()));
            }
            //finds the index of the object closest to the mouse
            if(!objVec.empty()){
                float minDistSqrd=static_cast<float>(sm.getCellSize())*static_cast<float>(sm.getCellSize());
                int minIndex=0;
                for(int i=0;i<objVec.size();i++){
                    float distSqrd=(mousePosf-objVec.at(i)->getPos()).lengthSquared();
                    if(distSqrd<minDistSqrd){
                        minDistSqrd=distSqrd;
                        minIndex=i;
                    }
                }
                if(selectedObject!=nullptr){
                    selectedObject->unhighlight();
                }
                selectedObject=objVec.at(minIndex);
                selectedObject->highlight();
            }else{
                if(selectedObject!=nullptr){
                    selectedObject->unhighlight();
                }
                selectedObject=nullptr;
            }
        }
    }

    

    //update all UI elements
    for(auto& element : UIElements){
        element->handleEvent(event);
    }
}

int Collisions::getPopulation(){
    return static_cast<int>(objects.size());
}

