#ifndef SPINNER_H
#define SPINNER_H

#include <SFML/Graphics.hpp>
#include "UI.h"
#include "Button.h"

class Spinner: public UI{
    private:
        float minValue;
        float maxValue;
        float currentValue;
        sf::Vector2f size;
        Button upButton;
        Button downButton;
        sf::Text label;

        bool editing=false;
        std::string input;

    public:
        Spinner(sf::Vector2f p, sf::Vector2f s, sf::Font& f, float min, float max, float value);

        void setValue(float value);

        void setText(const std::string& str);

        float getValue();

        void setRange(float min, float max);

        void handleEvent(const sf::Event& event) override;

        void draw(sf::RenderWindow& window) override;

        std::unique_ptr<UI> clone() const override;
};

#endif