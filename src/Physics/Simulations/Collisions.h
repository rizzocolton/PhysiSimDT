#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "../SpatialMap.h"
#include "../Objects/Circle.h"
#include "../../UI/UI.h"

class Collisions: public Simulation{
    private:
        float gravity;
        float colRestitution;
        float boundsRestitution;
        SpatialMap sm;
        sf::FloatRect simBounds;


        //Collection of physics objects in the simulation
        std::vector<std::unique_ptr<PhysicsObject>> objects;
        //Collection of UI elements
        std::vector<std::unique_ptr<UI>> UIElements;

        bool simulating=false;

    public:
        Collisions(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds);
        void update(float dt) override;
            
        void draw(sf::RenderWindow& window) override;

        void initUI(sf::Font& font) override;

        void drawUI(sf::RenderWindow& window) override;

        void handleEvent(const sf::Event& event) override;

        int getPopulation() override;

        ~Collisions() override = default;
};

#endif