#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "../../ObjectRenders.h"
#include "../SpatialHashMap.h"
#include "../../UI/UI.h"
#include "../../UI/Button.h"
#include "../../UI/Slider.h"
#include "../../UI/Spinner.h"
#include "../../UI/Label.h"


class Collisions: public Simulation{
    private:
        float gravity;
        float colRestitution;
        float boundsRestitution;
        bool showGrid=false;
        sf::FloatRect simBounds;
        std::function<void(SimType type)> switchSim;
        
       

    public:
        Collisions(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds, std::function<void(SimType type)> func, int maxEntities);

        //populating methods

        int createCircle(float x, float y, float r);

        //core methods

        void update(float dt) override;
            
        void draw(sf::RenderWindow& window) override;

        void initUI(sf::Font& font) override;

        void drawUI(sf::RenderWindow& window) override;

        void handleEvent(const sf::Event& event) override;

        int getPopulation() override;

        ~Collisions() override = default;
};

#endif