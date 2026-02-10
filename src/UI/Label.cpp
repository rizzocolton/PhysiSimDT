#include "Label.h"

Label::Label(const sf::Vector2f& p, sf::Font& font):UI(p), text(font){
    text.setFillColor(sf::Color::White);
    text.setPosition(pos);
}

Label::Label(const sf::Vector2f& p, sf::Font& font, int fontSize):UI(p), text(font){
    text.setFillColor(sf::Color::White);
    sf::FloatRect textBounds=text.getLocalBounds();
    text.setOrigin(textBounds.getCenter());
    text.setPosition(pos);
    text.setCharacterSize(fontSize);
}

void Label::setText(const std::string& str){
    text.setString(str);
}

void Label::handleEvent(const sf::Event& event){
    //Labels do not handle events
}

void Label::draw(sf::RenderWindow& window){
    window.draw(text);
}

std::unique_ptr<UI> Label::clone() const {
    return std::make_unique<Label>(*this);
}