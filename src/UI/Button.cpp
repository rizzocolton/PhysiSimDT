#include <iostream>
#include "Button.h"
#include "../Physics/Constants.h"

Button::Button(const sf::Vector2f& p, const sf::Vector2f& s):
text(ICELAND),pos(p),size(s),state(ButtonState::Normal),onClick([](){}){
    text.setFillColor(sf::Color::Black);
    shape.setPosition(pos);
    shape.setSize(size);
}



void Button::setText(const std::string& str){
    text.setString(str);

    sf::FloatRect textBounds=text.getLocalBounds();
    text.setOrigin(textBounds.getCenter());

    text.setPosition(
        sf::Vector2f{pos.x+size.x/2.f,pos.y+size.y/2.f}
    );
}

void Button::update(const sf::Vector2i& mousePos, bool mousePressed){
    //if mouse is within all bounds of the button
    bool isInside=(mousePos.x>pos.x&&mousePos.x<pos.x+size.x&&mousePos.y>pos.y&&mousePos.y<pos.y+size.y);

    
    if(isInside){
        if(mousePressed){
            //only changes state if state isn't already changed (prevents onClick from running repeatedly)
            if(state!=ButtonState::Pressed){
                state=ButtonState::Pressed;
                onClick();
            }
        }else if(state!=ButtonState::Hovered){
            state=ButtonState::Hovered;

        }
    }else if(state!=ButtonState::Normal){
        state=ButtonState::Normal;
    }
}

void Button::setOnClick(std::function<void()> func){
    onClick=func;
}

void Button::draw(sf::RenderWindow& window){
    window.draw(shape);
    window.draw(text);
}