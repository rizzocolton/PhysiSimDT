#ifndef EM_H
#define EM_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "../SpatialMap.h"
#include "../Objects/Circle.h"
#include "../../UI/UI.h"
#include "../../UI/Button.h"
#include "../../UI/Slider.h"
#include "../../UI/Spinner.h"
#include "../../UI/Label.h"




class EM: public Simulation{
    private:
        const float k=9E9f;
        float gravity;
        float colRestitution;
        float boundsRestitution;
        SpatialMap sm;
        bool showGrid=false;
        sf::FloatRect simBounds;
        std::function<void(SimType type)> switchSim;
        
       

    public:
        EM(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds, std::function<void(SimType type)> func);

        void update(float dt) override;
            
        void draw(sf::RenderWindow& window) override;

        void initUI(sf::Font& font) override;

        void drawUI(sf::RenderWindow& window) override;

        void handleEvent(const sf::Event& event) override;

        int getPopulation() override;

        ~EM() override = default;
};

#endif