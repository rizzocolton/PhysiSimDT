#include "Collisions.h"
#include <iostream>

Collisions::Collisions(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds, std::function<void(SimType type)> func, int maxEntities)
: gravity(gravity), colRestitution(colRestitution), simBounds(bounds),boundsRestitution(boundsRestitution),switchSim(func){
    simBounds.position.x/=scaleFactor;
    simBounds.position.y/=scaleFactor;
    simBounds.size.x/=scaleFactor;
    simBounds.size.y/=scaleFactor;

    state.maxx=simBounds.size.x;
    state.maxy=simBounds.size.y;
    state.reserve(maxEntities);

    for(int i=0;i<1;i++){
        int pId=state.spawnParticle(
            simBounds.size.x/2.f,
            1.f
        );
        objectShapes.push_back(std::make_unique<sf::CircleShape>(1.f));
        objectShapes.back()->setOrigin(sf::Vector2f(1.f,1.f));
    }
}

void Collisions::update(float dt){
    //If simulation is not running, skip physics update
    if(!simulating){
        return;
    }

    Systems::ZeroForces(state);
    Systems::GlobalGravity(state, gravity);
    Systems::Movement(state, dt);
    Systems::BoundaryCollisions(state, boundsRestitution);
    
    // 1/2 m v^2 + mgh = ME
    std::cout<<"ME "<<.5f*(1.f/state.invmass[0])*(pow(state.vx[0],2)+pow(state.vy[0],2))+(1.f/state.invmass[0])*-gravity*state.y[0]
             <<" at time "<<timeElapsed<<"s\n";

    timeElapsed+=dt; //add the amount of time elapsed in this frame
}

void Collisions::draw(sf::RenderWindow& window){
    //Optionally draw spatial map grid
    if(showGrid){
        
    }

    //Draw all objects, the index in objectShapes corresponds to the index of the physics object in state, so we can use the same index to get the position for drawing
    const auto start=std::chrono::steady_clock::now();
    for(int i=0;i<state.population;i++){
        objectShapes[i]->setPosition(sf::Vector2f(
            (state.x[i]+simBounds.position.x)*scaleFactor, 
            (simBounds.size.y-state.y[i]+simBounds.position.y)*scaleFactor
        ));
        window.draw(*objectShapes[i]);
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

    

    //update all UI elements
    for(auto& element : UIElements){
        element->handleEvent(event);
    }
}

int Collisions::getPopulation(){
    return state.population;
}

