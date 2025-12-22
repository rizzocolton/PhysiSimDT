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
    simSpace.setFillColor(sf::Color::Black);
    simSpace.setOutlineColor(sf::Color::White);
    simSpace.setOutlineThickness(-2.f);
    simSpace.setPosition(sf::Vector2f{500.f,0.f});

    sf::RectangleShape simControls{sf::Vector2f{500.f,600.f}};
    simControls.setFillColor(sf::Color::Black);
    simControls.setOutlineColor(sf::Color::White);
    simControls.setOutlineThickness(-2.f);
    simControls.setPosition(sf::Vector2f{0.f,0.f});

    sf::RectangleShape simObjectDetail{sf::Vector2f{500.f,400.f}};
    simObjectDetail.setFillColor(sf::Color::Black);
    simObjectDetail.setOutlineColor(sf::Color::White);
    simObjectDetail.setOutlineThickness(-2.f);
    simObjectDetail.setPosition(sf::Vector2f{0.f,600.f});

    sf::Font icelandFont{"../assets/Iceland-Regular.ttf"};

    //Simulation management
    std::unique_ptr<Simulation> currentSim = std::make_unique<Collisions>(
        9.8f, //gravity
        1.0f,  //collision restitution
        1.0f,  //bounds restitution
        20,    //cell size (in pixels)
        simSpace.getGlobalBounds() //simulation bounds
    );
    currentSim->initUI(icelandFont);

    //FPS and population counter

    sf::Text fpsCounter(icelandFont);
    fpsCounter.setPosition({500.f,0.f});
    sf::Text populationCounter(icelandFont);
    populationCounter.setPosition({500.f,20.f});

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
            if(event->is<sf::Event::KeyPressed>()){
                if(event->getIf<sf::Event::KeyPressed>()->scancode==sf::Keyboard::Scancode::Escape){
                    window.close();
                }
            }

            currentSim->handleEvent(*event);
        }

        //Do live updates for UI elements
        for(auto& element : currentSim->UIElements){
            //If element is a Spinner, run its live update
            Spinner* spinnerPtr = dynamic_cast<Spinner*>(element.get());
            Label* labelPtr = dynamic_cast<Label*>(element.get());
            if(spinnerPtr!=nullptr){
                spinnerPtr->runLiveUpdate();
            }
            if(labelPtr!=nullptr){
                labelPtr->runLiveUpdate();
            }
        }

        //Draw UI
        currentSim->drawUI(window);

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

        populationCounter.setString("Population: "+std::to_string(currentSim->getPopulation()));
        window.draw(populationCounter);

        window.display();
    }

}