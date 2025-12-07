#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    //creating display window
    sf::RenderWindow window(sf::VideoMode({1600, 1000}), "PhysiSim",sf::Style::Close);
    sf::RenderWindow controls(sf::VideoMode({500,500}),"Controls",sf::Style::Titlebar);

    //setup shapes
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()){

        //checks all window events that were triggered since last loop
        while (const std::optional event = window.pollEvent()){   
            //if close is requested, then close the window
            if (event->is<sf::Event::Closed>()){
                window.close();
                controls.close();
            }
        }

        while(const std::optional event = controls.pollEvent()){   
            
        }

        //* PRIMARY WINDOW RENDERING */

        //clear frame
        window.clear();
        //draw on frame


        window.draw(shape);

        //end frame
        window.display();

        //* CONTROL WINDOW RENDERING */

        controls.clear();
        
        controls.display();
    }
}