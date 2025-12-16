#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "../SpatialMap.h"
#include "../Objects/Circle.h"

class Collisions: public Simulation{
    private:
        float gravity;
        float colRestitution;
        float boundsRestitution;
        SpatialMap sm;
        std::vector<Circle> objects;
        sf::FloatRect simBounds;

    public:
        Collisions(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds);
        void update(float dt) override;
            
        void draw(sf::RenderWindow& window) override;

        void drawUI(sf::RenderWindow& window) override;

        void handleEvent(const sf::Event& event) override;

        ~Collisions() override = default;
};

#endif