#include "Spinner.h"

Spinner::Spinner(sf::Vector2f p, sf::Vector2f s, sf::Font& f, float min, float max, float value):
UI(p), 
upButton({p.x + s.x+5.f, p.y}, {20.f, s.y / 2.f}, f),
downButton({p.x + s.x+5.f, p.y + s.y / 2.f}, {20.f, s.y / 2.f}, f),
    size(s), label(f), minValue(min), maxValue(max), currentValue(value){
    if(currentValue<minValue) currentValue=minValue;
    if(currentValue>maxValue) currentValue=maxValue;

    upButton.setText("^");
    upButton.setOnChange([this]() {
        setValue(currentValue + 0.1f);
        runOnChange();
    });

    downButton.setText("^");
    downButton.rotateText(180.f);
    downButton.setOnChange([this]() {
        setValue(currentValue - 0.1f);
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

void Spinner::handleEvent(const sf::Event& event){
    if(event.getIf<sf::Event::MouseButtonPressed>() || event.getIf<sf::Event::MouseButtonReleased>() || event.getIf<sf::Event::MouseMoved>()){
        upButton.handleEvent(event);
        downButton.handleEvent(event);
        if(auto* click = event.getIf<sf::Event::MouseButtonPressed>()){
            sf::Vector2i mousePos = click->position;
            //check if mouse is over label
            if(mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
               mousePos.y >= pos.y && mousePos.y <= pos.y + size.y){
                //if mouse is clicking on label, need to enter edit mode to allow typing in a value
                editing=true;
                input="";
            }
        }
    }
    if(event.getIf<sf::Event::MouseWheelScrolled>()){
        const sf::Event::MouseWheelScrolled* wheelEvent = event.getIf<sf::Event::MouseWheelScrolled>();
        sf::Vector2i mousePos = wheelEvent->position;
        //check if mouse is over spinner
        if(mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
           mousePos.y >= pos.y && mousePos.y <= pos.y + size.y){
            //scroll up
            if(wheelEvent->delta > 0){
                setValue(currentValue*1.1f);
                runOnChange();
            }
            //scroll down
            else if(wheelEvent->delta < 0){
                setValue(currentValue*0.9f);
                runOnChange();
            }
        }
    }
    if(editing){
        if(auto* textEvent = event.getIf<sf::Event::TextEntered>()){
            if(textEvent->unicode >= '0' && textEvent->unicode <= '9' || textEvent->unicode == '.' || textEvent->unicode == '-'){
                input += static_cast<char>(textEvent->unicode);
            }else if(textEvent->unicode == '\b' && !input.empty()){
                input.pop_back();
            }else if(textEvent->unicode == '\r' && !input.empty()){
                try{
                    float value = std::stof(input);
                    setValue(value);
                    runOnChange();
                }catch(const std::exception& e){
                    //invalid input, ignore
                }
                editing=false;
            }
        }
    }
}

void Spinner::draw(sf::RenderWindow& window){
    //Draw main label
    if(editing){
        setText(input);
    }
    window.draw(label);

    //Draw buttons
    upButton.draw(window);
    downButton.draw(window);
}

std::unique_ptr<UI> Spinner::clone() const {
    return std::make_unique<Spinner>(*this);
}