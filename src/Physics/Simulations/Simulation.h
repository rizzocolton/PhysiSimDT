#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include "../Objects/PhysicsObject.h"
#include "../../UI/UI.h"

class Simulation{
    public:
         //Collection of physics objects in the simulation
        std::vector<std::unique_ptr<PhysicsObject>> objects;
        //Collection of UI elements
        std::vector<std::unique_ptr<UI>> UIElements;

        //Selected object for detail panel
        PhysicsObject* selectedObject=nullptr;

        bool simulating=false;    


        virtual void update(float dt)=0;
        virtual void draw(sf::RenderWindow& window)=0;
        virtual void initUI(sf::Font& font)=0;
        virtual void drawUI(sf::RenderWindow& window)=0;
        virtual void handleEvent(const sf::Event& event)=0;
        virtual int getPopulation()=0;
        virtual ~Simulation()=default;
};


#endif