#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>

enum class ButtonState{
    Normal,
    Hovered,
    Pressed
};

class Button{
private:
    sf::RectangleShape shape;
    sf::Vector2f pos;
    sf::Vector2f size;
    sf::Text text;
    ButtonState state;

    std::function<void()> onClick;
    

public:

    Button(const sf::Vector2f& p, const sf::Vector2f& s);

    void setText(const std::string& str);

    void update(const sf::Vector2i& mousePos, bool mousePressed);

    void setOnClick(std::function<void()> func);

    void draw(sf::RenderWindow& window);

};


#endif