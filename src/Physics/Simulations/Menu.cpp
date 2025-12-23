#include <SFML/Graphics.hpp>
#include "Menu.h"

Menu::Menu(std::function<void(SimType type)> func){
    switchSim=func;
}

void Menu::update(float dt){
    //no objects to update
}

void Menu::draw(sf::RenderWindow& window){
    //no objects to draw
}

//initUI defined in Siminits under UI

void Menu::drawUI(sf::RenderWindow& window){
    for(auto& element: UIElements){
        element->draw(window);
    }
}

void Menu::handleEvent(const sf::Event& event){
    //just pass all events to UI
    for(auto& element : UIElements){
        element->handleEvent(event);
    }
}

int Menu::getPopulation(){
    //no objects
    return 0;
}