#ifndef CIRCLE_H
#define CIRCLE_H

#include <SFML/Graphics.hpp>
#include "PhysicsObject.h"

class Circle : public PhysicsObject{
private:
    //The rendering of the circle
    sf::CircleShape shape;
    float radius;
    sf::Color color;

public:
    //Constructor (PhysicsObject parent)
    Circle(sf::Vector2f p, float r, float m, sf::Color color);

    //Virtual checkBounds override
    void checkBounds() override;

    //Rendering of circle onto screen
    void draw(sf::RenderWindow& window) override;

    //Collision detection/response with another Circle
    void collide(Circle& other);
};

#endif