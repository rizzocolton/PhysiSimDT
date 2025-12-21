#ifndef LABEL_H
#define LABEL_H

#include <SFML/Graphics.hpp>
#include "UI.h"

class Label: public UI{
    private:
        sf::Text text;
    public:
        Label(const sf::Vector2f& p, sf::Font& font);

        void setText(const std::string& str);

        void handleEvent(const sf::Event& event) override;

        void draw(sf::RenderWindow& window) override;
};

#endif