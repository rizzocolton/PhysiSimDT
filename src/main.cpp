#include <SFML/Graphics.hpp>
#include <iostream>
#include "Physics/Simulations/Menu.h"
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
    std::unique_ptr<Simulation> currentSim;
    std::unique_ptr<Simulation> nextSim;

    //lambda that the menu class can call to switch the sim. 
    //Menu has access to it because it's passed in to its constructor
    //need to declare what switchSim will be so that the lamdba can use it in itself
    //& not this because its in a function not object
    std::function<void(SimType)> switchSim;
    switchSim=[&](SimType type){
        switch(type){
            case SimType::Menu:
                nextSim= std::make_unique<Menu>(
                    switchSim
                );
                break;
            case SimType::Collisions:
                nextSim= std::make_unique<Collisions>(
                    9.8f, //gravity
                    1.0f,  //collision restitution
                    1.0f,  //bounds restitution
                    100,    //cell size (in pixels)
                    simSpace.getGlobalBounds(), //simulation bounds
                    switchSim //lambda allowing switching to the menu
                );
                break;
        }
        nextSim->initUI(icelandFont);
    };

    

    //start off on menu
    switchSim(SimType::Menu);
    currentSim=std::move(nextSim);

    //FPS and population counter

    sf::Text fpsCounter(icelandFont);
    fpsCounter.setPosition({500.f,0.f});
    sf::Text populationCounter(icelandFont);
    populationCounter.setPosition({500.f,20.f});

    int fps=0;
    sf::Clock fpsClock;
    sf::Clock deltaClock;
    
    while(window.isOpen()){

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

        //if next sim is loaded, make current sim point to it
        if(nextSim!=nullptr){
            currentSim=std::move(nextSim);
        }

        //boolean for seeing if current sim is pointing to the menu
        bool inMenu=dynamic_cast<Menu*>(currentSim.get())!=nullptr;

        window.clear();

        //Render all three sections of screen if active sim is not menu
        if(!inMenu){
            window.draw(simSpace);
            window.draw(simControls);
            window.draw(simObjectDetail);
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
        populationCounter.setString("Population: "+std::to_string(currentSim->getPopulation()));

        //draw fps and population if not in menu
        if(!inMenu){
            window.draw(fpsCounter);
            window.draw(populationCounter);
        }

        window.display();
    }

}