#include <iostream>
#include "Button.h"
#include "../Physics/Constants.h"

Button::Button(const sf::Vector2f& p, const sf::Vector2f& s):text(ICELAND){
    pos=p;
    size=s;
    shape.setPosition(pos);
    shape.setSize(size);
    state=ButtonState::Normal;
}

void Button::setText(const std::string& str){
    text.setString(str);
}

void Button::update(const sf::Vector2i& mousePos, bool mousePressed){
    //if mouse is within all bounds of the button
    bool isInside=(mousePos.x>pos.x&&mousePos.x<pos.x+size.x&&mousePos.y>pos.y&&mousePos.y<pos.y+size.y);

    
    if(isInside){
        if(mousePressed){
            state=ButtonState::Pressed;
            onClick();
        }else{
            state=ButtonState::Hovered;
        }
    }else{
        state=ButtonState::Normal;
    }
}

void Button::setOnClick(std::function<void()> func){
    onClick=func;
}

void Button::draw(sf::RenderWindow& window){
    window.draw(shape);
}