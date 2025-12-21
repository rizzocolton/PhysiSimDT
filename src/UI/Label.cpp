#include Label.h

Label::Label(const sf::Vector2f& p, sf::Font& font):UI(p), text(font){
    text.setFillColor(sf::Color::White);
    text.setPosition(pos);
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