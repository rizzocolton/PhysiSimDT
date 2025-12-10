#include <SFML/Graphics.hpp>
#include <iostream>
#include "Physics/Constants.h"
#include "Physics/SpatialMap.h"
#include "Physics/Objects/Circle.h"

int main(){

    //** SETUP */


    //DISPLAY SETUP

    //CONTROLS
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(SCREEN_WIDTH),static_cast<unsigned int>(SCREEN_HEIGHT)}), "PhysiSim",sf::Style::Close);
    window.setPosition({300,100});
    sf::RenderWindow controls(sf::VideoMode({300,800}),"Controls",sf::Style::Titlebar);
    controls.setPosition({0,100});

    //loading text and ui elements
    sf::Font font("../assets/Iceland-Regular.ttf");

    //vector to hold all controlsText
    std::vector<sf::Text> controlsText;

    //creating title
    sf::Text controlTitle(font);
    controlTitle.setString("Controls");
    controlTitle.setCharacterSize(24);
    controlTitle.setFillColor(sf::Color::White);
    controlsText.push_back(controlTitle);

    for(sf::Text text : controlsText){
        controls.draw(text);
    }
    controls.display();

    //SIMULATION
    sf::Text fpsCounter(font);
    fpsCounter.setPosition({1.f,0.f});

    sf::Text population(font);
    population.setPosition({1.f,20.f});
    
    //Objects container
    std::vector<Circle> objects;

    //* MAIN LOOP */
    int fps=0;
    sf::Clock oneSecond;
    sf::Clock timeSinceLastFrame;

    SpatialMap sm=SpatialMap(40);

    
    while(window.isOpen()){
        //checks all window events that were triggered since last loop
        while(const auto event = window.pollEvent()){   
            //if close is requested, then close the window
            if (event->is<sf::Event::Closed>()){
                window.close();
                controls.close();
            }
            //if escape is pressed, close all windows
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape){
                    window.close();
                    controls.close();
                }
            }
            //if the mouse is pressed, put an object at that positon
            if(const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                if(mouseButtonPressed->button==sf::Mouse::Button::Left){
                    sf::Vector2f mousePos(mouseButtonPressed->position.x,mouseButtonPressed->position.y);
                    objects.emplace_back(mousePos,rand()%10+1,rand()%10+1,sf::Color(rand()%255,rand()%255,rand()%255));
                }
            }
            
        }
        
        
        while(const std::optional event = controls.pollEvent()){   
            
        }

        //* PRIMARY WINDOW RENDERING */

        //clear frame
        window.clear();

        sm.draw(window);
        sm.clear();
        //time elapsed since last frame
        float dt=timeSinceLastFrame.restart().asSeconds();
        

        

        //updates and draws every object created
        for(auto& obj:objects){
            sf::Vector2f weight{0.0f,obj.getMass()*GRAVITY};
            obj.push(weight,dt);
            obj.update(dt);
            sm.enterCell(&obj);
            obj.draw(window);
        }

        for(auto& [cell,objs]: sm.getMap()){
            //makes a vector of all objs in set
            std::vector<Circle*> objVec(objs.begin(),objs.end());

            for(int i=0;i<objVec.size();i++){
                for(int j=i+1;j<objVec.size();j++){
                    objVec[i]->collide(*objVec[j]);
                }
            }
        }
        

        //end frame
        fps++;
        //display fps
        if(oneSecond.getElapsedTime().asSeconds()>=1){
            fpsCounter.setString("FPS: "+std::to_string(fps));
            oneSecond.restart();
            fps=0;
        }
        window.draw(fpsCounter);

        //display population
        population.setString("Population: "+std::to_string(objects.size()));
        window.draw(population);

        window.display();

        //* CONTROL WINDOW RENDERING */

        
    }
}