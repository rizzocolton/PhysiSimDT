#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"
#include "Circle.h"

int main(){

    //** SETUP */


    //DISPLAY SETUP

    //creating display & control windows 
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


    
    //Objects container
    std::vector<Circle> objects;

    //* MAIN LOOP */
    sf::Clock clock;
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
                    objects.emplace_back(mousePos,40,1,sf::Color::Red);
                }
            }
            
        }

        while(const std::optional event = controls.pollEvent()){   
            
        }

        //* PRIMARY WINDOW RENDERING */

        //clear frame
        window.clear();

        //time elapsed since last frame
        float dt=clock.restart().asSeconds();
        

        //updates and draws every object created
        for(auto& obj:objects){
            sf::Vector2f weight{0.0f,obj.getMass()*GRAVITY};
            obj.push(weight,dt);
            obj.update(dt);
            for(auto& otherObj:objects){
                obj.collide(otherObj);
            }
            obj.draw(window);
        }

        //end frame
        window.display();

        //* CONTROL WINDOW RENDERING */

        
    }
}