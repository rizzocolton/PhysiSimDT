#include "../../Physics/Simulations/Menu.h"
#include "../Button.h"
#include "../Label.h"
#include <iostream>

void Menu::initUI(sf::Font& font){
    Label* titleLabel = new Label({860.f,20.f},font, 50);
    titleLabel->setText("PhysiSim");
    UIElements.push_back(std::unique_ptr<Label>(titleLabel));

    Button* collisionsButton = new Button({860.f,100.f},{300.f,50.f},font);
    collisionsButton->setText("Collisions");
    //need to capture switching function by value instead of reference, otherwise the lambda may delete itself!
    collisionsButton->setOnChange([sm=this->switchSim](){
        sm(SimType::Collisions);
    });
    UIElements.push_back(std::unique_ptr<Button>(collisionsButton));

    Button* gravityButton = new Button({860.f, 170.f},{300.f,50.f},font);
    gravityButton->setText("Gravity");
    gravityButton->setOnChange([sm=this->switchSim](){
        sm(SimType::Gravity);
    });
    UIElements.push_back(std::unique_ptr<Button>(gravityButton));

    Button* EMButton = new Button({860.f, 240.f},{300.f,50.f},font);
    EMButton->setText("Electricity & Magnetism");
    EMButton->setOnChange([sm=this->switchSim](){
        sm(SimType::EM);
    });
    UIElements.push_back(std::unique_ptr<Button>(EMButton));
}