#ifndef OBJECTRENDERMETHODS_H
#define OBJECTRENDERMETHODS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

//this class will hold all of the techniques to get opitmized shapes (in form of vertex arrays) put into one huge vertex array for drawing.
class ObjectRenderer{
    private:
        static std::vector<sf::Vertex> vertices; //vertex array for drawing all objects in the simulation, will be cleared and repopulated every frame
        static sf::Texture circleTexture; //master texture for circles, will be scaled and colored using vertex shader to create all circle objects in the simulation
    public:
        static void initTexture(); //initializes the master circle texture, should be called once at the start of the program

        //clears vertices for new frame
        static void clear();
        
        static void addCircle(float x, float y, float radius, sf::Color color, float scaleFactor, float xOffset, float yOffset);


        static void draw(sf::RenderWindow& window);
};

#endif