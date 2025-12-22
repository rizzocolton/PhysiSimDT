#include "Spinner.h"

Spinner::Spinner(sf::Vector2f p, sf::Vector2f s, sf::Font& f, float min, float max, float value):
UI(p), 
upButton({p.x + s.x+5.f, p.y}, {20.f, s.y / 2.f}, f),
downButton({p.x + s.x+5.f, p.y + s.y / 2.f}, {20.f, s.y / 2.f}, f),
    size(s), label(f), minValue(min), maxValue(max), currentValue(value){
    if(currentValue<minValue) currentValue=minValue;
    if(currentValue>maxValue) currentValue=maxValue;

    upButton.setText("^");
    upButton.setOnClick([this]() {
        setValue(currentValue + 0.01f);
        runOnChange();
    });

    downButton.setText("^");
    downButton.rotateText(180.f);
    downButton.setOnClick([this]() {
        setValue(currentValue - 0.01f);
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
    label.setOrigin({textBounds.size.x,0.f});
    label.setPosition(
        sf::Vector2f{pos.x+size.x,pos.y}
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
    if(event.getIf<sf::Event::MouseWheelScrolled>()){
        const sf::Event::MouseWheelScrolled* wheelEvent = event.getIf<sf::Event::MouseWheelScrolled>();
        sf::Vector2i mousePos = wheelEvent->position;
        //check if mouse is over spinner
        if(mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
           mousePos.y >= pos.y && mousePos.y <= pos.y + size.y){
            //scroll up
            if(wheelEvent->delta > 0){
                setValue(currentValue + 0.1f);
                runOnChange();
            }
            //scroll down
            else if(wheelEvent->delta < 0){
                setValue(currentValue - 0.1f);
                runOnChange();
            }
        }
    }
}

void Spinner::draw(sf::RenderWindow& window){
    //Draw main label
    window.draw(label);

    //Draw buttons
    upButton.draw(window);
    downButton.draw(window);
}