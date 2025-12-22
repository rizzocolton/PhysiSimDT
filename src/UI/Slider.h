#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>
#include <functional>
#include "UI.h"

class Slider: public UI{
private:
    float minValue;
    float maxValue;
    float currentValue;
    sf::Vector2f size;
    sf::RectangleShape filledBar;
    sf::RectangleShape bar;
    sf::Text label;

    std::function<void()> onChange;

public:
    Slider(sf::Vector2f p, sf::Vector2f s, sf::Font& f, float min, float max, float value);

    void setValue(float value);

    void setText(const std::string& str);

    float getValue();

    void setRange(float min, float max);

    void setOnChange(std::function<void()> func);

    void runOnChange();

    void handleEvent(const sf::Event& event) override;

    void draw(sf::RenderWindow& window) override;

    std::unique_ptr<UI> clone() const override;
};

#endif