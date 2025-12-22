#ifndef SPINNER_H
#define SPINNER_H

#include <SFML/Graphics.hpp>
#include <functional>
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
        std::function<void()> onChange;
        std::function<void()> liveUpdate;

    public:
        Spinner(sf::Vector2f p, sf::Vector2f s, sf::Font& f, float min, float max, float value);

        void setValue(float value);

        void setText(const std::string& str);

        float getValue();

        void setRange(float min, float max);

        void setOnChange(std::function<void()> func);

        void runOnChange();

        void setLiveUpdate(std::function<void()> func);

        void runLiveUpdate();

        void handleEvent(const sf::Event& event) override;

        void draw(sf::RenderWindow& window) override;

        std::unique_ptr<UI> clone() const override;
};

#endif