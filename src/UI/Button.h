#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>
#include "UI.h"

enum class ButtonState{
    Normal,
    Hovered,
    Pressed
};

class Button: public UI{
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    sf::Vector2f size;
    ButtonState state;

    std::function<void()> onClick;
    

public:

    Button(const sf::Vector2f& p, const sf::Vector2f& s, sf::Font& font);

    void setText(const std::string& str);

    void rotateText(float angle);

    void handleEvent(const sf::Event& event) override;

    void setOnClick(std::function<void()> func);

    void runOnClick();

    void draw(sf::RenderWindow& window) override;

};


#endif