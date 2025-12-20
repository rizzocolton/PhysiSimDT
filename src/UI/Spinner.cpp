#include "Spinner.h"

Spinner::Spinner(sf::Vector2f p, sf::Vector2f s, sf::Font& f, float min, float max, float value):
UI(p), 
upButton({p.x + s.x, p.y}, {s.x / 2.f, s.y / 2.f}, f),
downButton({p.x + s.x, p.y + s.y / 2.f}, {s.x / 2.f, s.y / 2.f}, f),
    size(s), label(f), minValue(min), maxValue(max), currentValue(value){
    if(currentValue<minValue) currentValue=minValue;
    if(currentValue>maxValue) currentValue=maxValue;

    upButton.setText("^");
    upButton.setOnClick([this]() {
        setValue(currentValue + 1.f);
        runOnChange();
    });

    downButton.setText("^");
    downButton.rotateText(180.f);
    downButton.setOnClick([this]() {
        setValue(currentValue - 1.f);
        runOnChange();
    });
};

void Spinner::setValue(float value){
    if(value<minValue) currentValue=minValue;
    else if(value>maxValue) currentValue=maxValue;
    else currentValue=value;
}

void Spinner::setText(const std::string& str){
    label.setString(str);

    sf::FloatRect textBounds=label.getLocalBounds();
    label.setOrigin(textBounds.getCenter());

    label.setPosition(
        sf::Vector2f{pos.x+size.x/2.f,pos.y+size.y/2.f}
    );
}

float Spinner::getValue(){
    return currentValue;
}

void Spinner::setRange(float min, float max){
    minValue=min;
    maxValue=max;
    if(currentValue<minValue) currentValue=minValue;
    if(currentValue>maxValue) currentValue=maxValue;
}

void Spinner::setOnChange(std::function<void()> func){
    onChange=func;
}

void Spinner::runOnChange(){
    onChange();
}

void Spinner::setLiveUpdate(std::function<void()> func){
    liveUpdate=func;
}

void Spinner::runLiveUpdate(){
    liveUpdate();
}

void Spinner::handleEvent(const sf::Event& event){
    if(event.getIf<sf::Event::MouseButtonPressed>() || event.getIf<sf::Event::MouseButtonReleased>() || event.getIf<sf::Event::MouseMoved>()){
        upButton.handleEvent(event);
        downButton.handleEvent(event);
    }
}

void Spinner::draw(sf::RenderWindow& window){
    //Draw main label
    label.setPosition(
        sf::Vector2f{pos.x+size.x/2.f,pos.y+size.y/2.f}
    );
    window.draw(label);

    //Draw buttons
    upButton.draw(window);
    downButton.draw(window);
}