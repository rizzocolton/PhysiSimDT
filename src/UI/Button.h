#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Button{
private:
    sf::RectangleShape shape;

public:

    Button(sf::Vector2f pos,sf::Vector2f size);

}


#endif