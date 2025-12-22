#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>

class UI{
    protected:
        sf::Vector2f pos;
    public:
        UI(sf::Vector2f p): pos(p) {};

        virtual void draw(sf::RenderWindow& window)=0;

        virtual void handleEvent(const sf::Event& event)=0;

        //for copying into saves
        virtual std::unique_ptr<UI> clone() const=0;

        virtual ~UI()=default;
};

#endif