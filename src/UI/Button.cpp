#include <iostream>
#include "Button.h"
#include "../Physics/Constants.h"

Button::Button(const sf::Vector2f& p, const sf::Vector2f& s, sf::Font& font):UI(p),
text(font),size(s),state(ButtonState::Normal),onClick([](){}){
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

void Button::rotateText(float angle){
    text.rotate(sf::Angle{sf::degrees(angle)});
}

void Button::handleEvent(const sf::Event& event){
    sf::Vector2i mousePos=sf::Mouse::getPosition();
    bool mousePressed=sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    if(event.getIf<sf::Event::MouseMoved>()){
        if(mousePos.x>=pos.x && mousePos.x<=pos.x+size.x &&
           mousePos.y>=pos.y && mousePos.y<=pos.y+size.y){
            state=ButtonState::Hovered;
        } else {
            state=ButtonState::Normal;
        }
    }

    if(event.getIf<sf::Event::MouseButtonPressed>()){
        if(mousePos.x>=pos.x && mousePos.x<=pos.x+size.x &&
           mousePos.y>=pos.y && mousePos.y<=pos.y+size.y){
            state=ButtonState::Pressed;
            runOnClick();
        }
    }

    if(event.getIf<sf::Event::MouseButtonReleased>()){
        if(state==ButtonState::Pressed){
            if(mousePos.x>=pos.x && mousePos.x<=pos.x+size.x &&
               mousePos.y>=pos.y && mousePos.y<=pos.y+size.y){
                state=ButtonState::Hovered;
            } else {
                state=ButtonState::Normal;
            }
        }
    }
}

void Button::setOnClick(std::function<void()> func){
    onClick=func;
}

void Button::runOnClick(){
    onClick();
}

void Button::draw(sf::RenderWindow& window){
    switch(state){
        case ButtonState::Normal:
            shape.setFillColor(sf::Color::White);
            break;
        case ButtonState::Hovered:
            shape.setFillColor(sf::Color(200,200,200));
            break;
    }
    window.draw(shape);
    window.draw(text);
}