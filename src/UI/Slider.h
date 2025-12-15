#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>

class Slider{
private:
    float minValue;
    float maxValue;
    float currentValue;
    sf::Vector2f pos;
    sf::Vector2f size;
    sf::RectangleShape filledBar;
    sf::RectangleShape bar;

public:
    Slider(sf::Vector2f p, sf::Vector2f s, float min, float max, float value);

    void setValue(float value);

    float getValue();

    void setRange(float min, float max);

    bool update(sf::Vector2i mousePos, bool mousePressed);

    void draw(sf::RenderWindow& window);
};

#endif