#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "../SpatialMap.h"
#include "../Objects/Circle.h"
#include "../../UI/UI.h"
#include "../../UI/Button.h"
#include "../../UI/Slider.h"
#include "../../UI/Spinner.h"
#include "../../UI/Label.h"


//all in pixel units
struct SpawnParams{
    float radius;
    float mass;
    sf::Color color;
    float vx;
    float vy;
};

class Collisions: public Simulation{
    private:
        float gravity;
        float colRestitution;
        float boundsRestitution;
        SpatialMap sm;
        sf::FloatRect simBounds;
        std::function<void(SimType type)> switchSim;
        SpawnParams params;
       

    public:
        Collisions(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds, std::function<void(SimType type)> func);

        void update(float dt) override;
            
        void draw(sf::RenderWindow& window) override;

        void initUI(sf::Font& font) override;

        void drawUI(sf::RenderWindow& window) override;

        void handleEvent(const sf::Event& event) override;

        int getPopulation() override;

        ~Collisions() override = default;
};

#endif