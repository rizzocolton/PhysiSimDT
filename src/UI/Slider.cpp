#include "Slider.h"
#include "../Physics/Constants.h"

Slider::Slider(sf::Vector2f p, sf::Vector2f s, float min, float max, float value):UI(p),
    size(s), minValue(min), maxValue(max), currentValue(value){
    if(currentValue<minValue) currentValue=minValue;
    if(currentValue>maxValue) currentValue=maxValue;
};

void Slider::setValue(float value){
    if(value<minValue) currentValue=minValue;
    else if(value>maxValue) currentValue=maxValue;
    else currentValue=value;
}

float Slider::getValue(){
    return currentValue;
}

void Slider::setRange(float min, float max){
    minValue=min;
    maxValue=max;
    if(currentValue<minValue) currentValue=minValue;
    if(currentValue>maxValue) currentValue=maxValue;
}

void Slider::setOnChange(std::function<void()> func){
    onChange=func;
}

void Slider::runOnChange(){
    onChange();
}

void Slider::handleEvent(const sf::Event& event){
    sf::Vector2i mousePos=sf::Mouse::getPosition();
    bool mousePressed=sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    bool isInside=(mousePos.x>=pos.x && mousePos.x<=pos.x+size.x &&
                   mousePos.y>=pos.y && mousePos.y<=pos.y+size.y);

    if(isInside&&mousePressed){
        float percent=(mousePos.x - pos.x)/size.x;
        float value=minValue+percent*(maxValue - minValue);
        setValue(value);
        runOnChange();
    }
}

void Slider::draw(sf::RenderWindow& window){
    bar.setSize(size);
    bar.setFillColor(sf::Color(150,150,150));
    bar.setPosition(pos);
    window.draw(bar);

    float filledWidth=size.x*((currentValue - minValue)/(float)(maxValue - minValue));
    filledBar.setSize({filledWidth,size.y});
    filledBar.setFillColor(sf::Color(100,100,250));
    filledBar.setPosition(pos);
    window.draw(filledBar);
}