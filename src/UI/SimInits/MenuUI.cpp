#include "../../Physics/Simulations/Menu.h"
#include "../Button.h"
#include "../Label.h"
#include <iostream>

void Menu::initUI(sf::Font& font){
    Button* collisionsButton = new Button({600.f,10.f},{150.f,50.f},font);
    collisionsButton->setText("Collisions");
    collisionsButton->setOnClick([sm=this->switchSim](){
        std::cout << "Lambda called!" << std::endl;
        sm(SimType::Collisions);
        std::cout << "After switch" << std::endl;
    });
    UIElements.push_back(std::unique_ptr<Button>(collisionsButton));
}