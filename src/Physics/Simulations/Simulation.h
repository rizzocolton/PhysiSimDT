#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include <functional>
#include "../Objects/PhysicsObject.h"
#include "../../UI/UI.h"

struct SaveState{
    std::vector<std::unique_ptr<PhysicsObject>> savedObjects;
    std::vector<std::unique_ptr<UI>> savedUIElements;
};

enum class SimType{
    Menu,
    Collisions
};


class Simulation{
    public:
         //Collection of physics objects in the simulation
        std::vector<std::unique_ptr<PhysicsObject>> objects;
        //Collection of UI elements
        std::vector<std::unique_ptr<UI>> UIElements;

        //a save state
        SaveState save;

        //Selected object for detail panel
        PhysicsObject* selectedObject=nullptr;

        bool simulating=false;
        
        float scaleFactor=100.f; //100 pixels = 1 meter
        float timeFactor=1.f;  //real-time by default
         


        virtual void update(float dt)=0;
        virtual void draw(sf::RenderWindow& window)=0;
        virtual void initUI(sf::Font& font)=0;
        virtual void drawUI(sf::RenderWindow& window)=0;
        virtual void handleEvent(const sf::Event& event)=0;
        virtual int getPopulation()=0;
        virtual ~Simulation()=default;
};


#endif