#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>

class UI{
    protected:
        sf::Vector2f pos;
    public:
        UI(sf::Vector2f p);

        virtual void draw(sf::RenderWindow& window)=0;

        virtual void handleEvent(const sf::Event& event)=0;

        virtual ~UI()=default;
}

#endif