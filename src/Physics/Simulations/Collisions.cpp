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

    ObjectRenderer::initTexture();

    for(int i=0;i<maxEntities;i++){
        int id=createCircle(
            (float)rand()/RAND_MAX*simBounds.size.x,
            (float)rand()/RAND_MAX*simBounds.size.y,
            2.f*(1.f/scaleFactor)
        );
        state.vx[id]=i%5-2;
        state.vy[id]=i%5-2;
    }
}

int Collisions::createCircle(float x, float y, float r){
    int particleId=state.spawnParticle(x,y);
    state.hasRadius.push_back(particleId);
    state.radius.push_back(r);

    return particleId;
}

void Collisions::update(float dt){
    //If simulation is not running, skip physics update
    if(!simulating){
        return;
    }

    auto start=std::chrono::steady_clock::now();

    Systems::ZeroForces(state);
    Systems::GlobalGravity(state, gravity);
    Systems::Movement(state, dt);

    auto colS=std::chrono::steady_clock::now();
    Systems::Collisions(state, colRestitution);
    auto colE=std::chrono::steady_clock::now();
    std::cout<<"Collisions Took: "<<std::chrono::duration_cast<std::chrono::microseconds>(colE-colS).count()<<"us\n";

    Systems::BoundaryCollisions(state, boundsRestitution);
    
    
    auto end=std::chrono::steady_clock::now();
    //std::cout<<"Physics Update Took: "<<std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()<<"us\n";

    timeElapsed+=dt; //add the amount of time elapsed in this frame
}

void Collisions::draw(sf::RenderWindow& window){
    //Optionally draw spatial map grid
    if(showGrid){
        
    }
    ObjectRenderer::clear();
    
    //Add all circles to draw
    for(int i=0; i<state.hasRadius.size(); i++){
        int particleId=state.hasRadius[i];
        ObjectRenderer::addCircle(state.x[particleId], (simBounds.size.y-state.y[particleId]), state.radius[i], sf::Color::White, scaleFactor, simBounds.position.x, simBounds.position.y);
    }
    ObjectRenderer::draw(window);
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

