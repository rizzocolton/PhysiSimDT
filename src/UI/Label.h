#ifndef LABEL_H
#define LABEL_H

#include <SFML/Graphics.hpp>
#include "UI.h"
#include <functional>

class Label: public UI{
    private:
        sf::Text text;
        std::function<void()> liveUpdate;
    public:
        Label(const sf::Vector2f& p, sf::Font& font);

        void setText(const std::string& str);

        void setLiveUpdate(std::function<void()> func);

        void runLiveUpdate();

        void handleEvent(const sf::Event& event) override;

        void draw(sf::RenderWindow& window) override;
};

#endif