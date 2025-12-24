#ifndef GRAVITY_H
#define GRAVITY_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "../SpatialMap.h"
#include "../Objects/Circle.h"
#include "../../UI/UI.h"
#include "../../UI/Button.h"
#include "../../UI/Slider.h"
#include "../../UI/Spinner.h"
#include "../../UI/Label.h"

class Gravity: public Simulation{
    private:
        const float G=6.6743e-11f;
        float colRestitution;
        SpatialMap sm;
        sf::FloatRect simBounds;
        std::function<void(SimType type)> switchSim;
       

    public:
        Gravity(float colRestitution, int cellSize, sf::FloatRect bounds, std::function<void(SimType type)> func);

        void update(float dt) override;
            
        void draw(sf::RenderWindow& window) override;

        void initUI(sf::Font& font) override;

        void drawUI(sf::RenderWindow& window) override;

        void handleEvent(const sf::Event& event) override;

        int getPopulation() override;

        ~Gravity() override = default;
};



#endif