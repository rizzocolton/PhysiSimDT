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

    // Load font with error checking
    sf::Font icelandFont;
    if (!icelandFont.openFromFile("assets/Iceland-Regular.ttf")) {
        std::cerr << "ERROR: Failed to load font from assets/Iceland-Regular.ttf" << std::endl;
        return 1;
    }

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
                    -9.81f, //gravity
                    1.f,  //collision restitution
                    1.0f,  //bounds restitution
                    1,    //cell size (in meters)
                    simSpace.getGlobalBounds(), //simulation bounds (in pixels, but will be converted to meters in the sim constructor)
                    switchSim, //lambda allowing switching to the menu
                    1000 //max entities
                );
                break;
        }
        nextSim->initUI(icelandFont);
    };

    

    //start off on menu
    switchSim(SimType::Menu);
    currentSim=std::move(nextSim);

    //FPS, time elapsed, and population counter
    sf::Text timeCounter(icelandFont);
    timeCounter.setPosition({500.f,0.f});
    sf::Text fpsCounter(icelandFont);
    fpsCounter.setPosition({500.f,20.f});
    sf::Text populationCounter(icelandFont);
    populationCounter.setPosition({500.f,40.f});

    int fps=0;
    sf::Clock fpsClock;
    sf::Clock deltaClock;
    
    float fixedDT=1/120.f;
    float accumulator=0;

    
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

        //Render sim space if active sim is not menu
        if(!inMenu){
            window.draw(simSpace);
        }
        
        //Do live updates for UI elements
        for(auto& element : currentSim->UIElements){
            element->runLiveUpdate();
        }

        //Update simulation in fixed time-steps
        float dt=deltaClock.restart().asSeconds();
        accumulator+=dt*currentSim->timeFactor;

        int iterations=0;
        while(accumulator>=fixedDT&&iterations<10){ //cap iterations to prevent spiral of death
            currentSim->update(fixedDT);
            accumulator-=fixedDT;
            iterations++;
        }
        currentSim->draw(window);
        if(currentSim->simulating && accumulator>=fixedDT){
            sf::Text warningText(icelandFont);
            warningText.setFillColor(sf::Color::Red);
            warningText.setString("Warning: Simulation is running slowly! Consider reducing simulation speed");
            warningText.setPosition({1300.f,0.f});
            window.draw(warningText);
        }

        fps++;
        if(fpsClock.getElapsedTime().asSeconds()>=1.f){
            fpsCounter.setString("FPS: "+std::to_string(fps));
            fps=0;
            fpsClock.restart();
        }
        timeCounter.setString("Time Elapsed: "+std::to_string(currentSim->timeElapsed));
        populationCounter.setString("Population: "+std::to_string(currentSim->getPopulation()));

        //draw ui and stats if menu is not active
        if(!inMenu){
            window.draw(simControls);
            window.draw(simObjectDetail);
            window.draw(fpsCounter);
            window.draw(timeCounter);
            window.draw(populationCounter);
        }
        currentSim->drawUI(window);

        window.display();
    }

}