#include <SFML/Graphics.hpp>
#include <iostream>
#include "Physics/Simulations/Collisions.h"

int main(){

    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "PhysiSim",
        sf::State::Fullscreen
    );

    sf::RectangleShape simSpace{sf::Vector2f{1400.f,1000.f}};
    simSpace.setFillColor(sf::Color::Green);
    simSpace.setPosition(sf::Vector2f{500.f,0.f});

    sf::RectangleShape simControls{sf::Vector2f{500.f,600.f}};
    simControls.setFillColor(sf::Color::Blue);
    simControls.setPosition(sf::Vector2f{0.f,0.f});

    sf::RectangleShape simObjectDetail{sf::Vector2f{500.f,400.f}};
    simObjectDetail.setFillColor(sf::Color::Red);
    simObjectDetail.setPosition(sf::Vector2f{0.f,600.f});

    sf::Font icelandFont{"../assets/Iceland-Regular.ttf"};

    //Simulation management
    std::unique_ptr<Simulation> currentSim = std::make_unique<Collisions>(
        98.f, //gravity
        1.0f,  //collision restitution
        1.0f,  //bounds restitution
        50,    //cell size
        simSpace.getGlobalBounds() //simulation bounds
    );

    sf::Text fpsCounter(icelandFont);
    fpsCounter.setPosition({500.f,10.f});
    int fps=0;
    sf::Clock fpsClock;
    sf::Clock deltaClock;
    
    while(window.isOpen()){
        window.clear();

        //Render all three sections
        window.draw(simSpace);
        window.draw(simControls);
        window.draw(simObjectDetail);

        //Handle events

        while(const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }


            currentSim->handleEvent(*event);
        }

        //Update simulation
        float dt=deltaClock.restart().asSeconds();

        currentSim->update(dt);
        currentSim->draw(window);

        fps++;
        if(fpsClock.getElapsedTime().asSeconds()>=1.f){
            fpsCounter.setString("FPS: "+std::to_string(fps));
            fps=0;
            fpsClock.restart();
        }
        window.draw(fpsCounter);

        window.display();
    }

}