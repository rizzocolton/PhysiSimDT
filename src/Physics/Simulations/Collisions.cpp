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

    state.x[0]=simBounds.size.x/2.f;
    state.y[0]=simBounds.size.y/2.f;
    state.vx[0]=0.f;
    state.vy[0]=0.f;
    state.invmass[0]=1.f;
    state.population=1;
}

void Collisions::update(float dt){
    //If simulation is not running, skip physics update
    if(!simulating){
        return;
    }

    std::cout<<"Current State:\n";
    for(int i=0; i<state.population; i++){
        std::cout<<"Object "<<i<<": x="<<state.x[i]<<", y="<<state.y[i]<<", vx="<<state.vx[i]<<", vy="<<state.vy[i]<<", time="<<timeElapsed<<"\n";
    }

    Systems::ZeroForces(state);
    Systems::GlobalGravity(state, gravity);
    Systems::Movement(state, dt);
    Systems::BoundaryCollisions(state, boundsRestitution);

    timeElapsed+=dt; //add the amount of time elapsed in this frame
}

void Collisions::draw(sf::RenderWindow& window){
    //Optionally draw spatial map grid
    if(showGrid){
        
    }

    //Draw all objects
    for(int i=0;i<state.population;i++){
        sf::CircleShape shape;
        shape.setRadius(10.f);
        shape.setOrigin(sf::Vector2f(10.f,10.f));
        shape.setPosition(sf::Vector2f(
            (state.x[i]+simBounds.position.x)*scaleFactor,
            (simBounds.size.y-state.y[i]+simBounds.position.y)*scaleFactor
        ));
        window.draw(shape);
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

