#include "../../Physics/Simulations/Menu.h"
#include "../Button.h"
#include "../Label.h"
#include <iostream>

void Menu::initUI(sf::Font& font){

    Button* collisionsButton = new Button({860.f,100.f},{200.f,50.f},font);
    collisionsButton->setText("Collisions");
    //need to capture switching function by value instead of reference, otherwise the lambda may delete itself!
    collisionsButton->setOnChange([sm=this->switchSim](){
        sm(SimType::Collisions);
    });
    UIElements.push_back(std::unique_ptr<Button>(collisionsButton));

    Button* gravityButton = new Button({860.f, 150.f},{200.f,50.f},font);
    gravityButton->setText("Gravity");
    gravityButton->setOnChange([sm=this->switchSim](){
        sm(SimType::Gravity);
    });
    UIElements.push_back(std::unique_ptr<Button>(gravityButton));
}