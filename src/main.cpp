#include <SFML/Graphics.hpp>
#include <iostream>
#include "Physics/Constants.h"
#include "Physics/SpatialMap.h"
#include "Physics/Objects/Circle.h"
#include "UI/Button.h"

int main(){

    //** SETUP */


    //WINDOWS

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(SCREEN_WIDTH),static_cast<unsigned int>(SCREEN_HEIGHT)}), "PhysiSim",sf::Style::Close);
    window.setPosition({300,100});
    sf::RenderWindow controls(sf::VideoMode({300,800}),"Controls",sf::Style::Titlebar);
    controls.setPosition({0,100});

    
    //SIMULATION SCREEN

    loadFonts();
    sf::Text fpsCounter(ICELAND);
    fpsCounter.setPosition({1.f,0.f});

    sf::Text population(ICELAND);
    population.setPosition({1.f,20.f});

    //SIMULATION VARIABLES
    
    //Objects container
    bool simulating=false;
    std::vector<Circle> objects;
    int fps=0;
    sf::Clock oneSecond;
    sf::Clock timeSinceLastFrame;
    SpatialMap sm=SpatialMap(40);
    

    //CONTROLS SCREEN

     sf::Vector2i mousePos;

    Button start_Stop{sf::Vector2f{10.f,50.f}, sf::Vector2f{200.f,200.f}};
    start_Stop.setText("Start");
    start_Stop.setOnClick([&simulating,&start_Stop](){
        simulating=!simulating;
        if(simulating){
            start_Stop.setText("Stop");
        }else{
            start_Stop.setText("Start");
        }
    });
    

    //* MAIN LOOP */
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
                    for(int i=0;i<10;i++){
                        objects.emplace_back(mousePos,10,rand()%10+1,sf::Color(rand()%255,rand()%255,rand()%255));
                    }
                    
                }
            }
            
        }
        
        
        while(const std::optional event = controls.pollEvent()){   
        }

        //FIND MOUSE LOCATION; USEFUL FOR MANY INTERACTIONS
        mousePos=sf::Mouse::getPosition(controls);

        //* PRIMARY WINDOW RENDERING */

        //SIMULATION

        window.clear();
        sm.draw(window);
        sm.clear();
        //time elapsed since last frame
        float dt=timeSinceLastFrame.restart().asSeconds();
        if(!simulating){
            dt=0.f; //Nothing will simulate if no time has passed.
        }

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
        }//end simulation frame
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
        
        controls.clear();

        bool mouseClicked=sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        start_Stop.update(mousePos,mouseClicked);
        start_Stop.draw(controls);


        controls.display();
        
    }
}