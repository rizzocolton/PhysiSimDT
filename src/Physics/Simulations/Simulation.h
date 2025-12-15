#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>

class Simulation{
    public:
        virtual void update(float dt)=0;
        virtual void draw(sf::RenderWindow& window)=0;
        virtual void drawUI(sf::RenderWindow& controls)=0;
        virtual void handleEvent(const sf::Event& event)=0;
        virtual ~Simulation()=default;

};


#endif