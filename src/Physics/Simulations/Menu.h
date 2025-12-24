#ifndef MENU_H
#define MENU_H

#include "Simulation.h"




class Menu: public Simulation{
public:
    std::function<void(SimType type)> switchSim;

    Menu(std::function<void(SimType type)> func);

    void update(float dt) override;
        
    void draw(sf::RenderWindow& window) override;

    void initUI(sf::Font& font) override;

    void drawUI(sf::RenderWindow& window) override;

    void handleEvent(const sf::Event& event) override;

    int getPopulation() override;

    ~Menu() override = default;
};

#endif