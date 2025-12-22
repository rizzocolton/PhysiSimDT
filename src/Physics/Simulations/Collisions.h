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
#include <iomanip>
#include <sstream>

class Collisions: public Simulation{
    private:
        float gravity;
        float colRestitution;
        float boundsRestitution;
        SpatialMap sm;
        sf::FloatRect simBounds;
        
       

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

inline std::string formatFloatToSigFigs(float value, int sigfigs){
    //formats a float to specified # of significant figures
    std::stringstream stream;
    stream << std::setprecision(sigfigs) << value;
    return stream.str();
}

#endif