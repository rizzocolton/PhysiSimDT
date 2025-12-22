#ifndef CIRCLE_H
#define CIRCLE_H

#include <SFML/Graphics.hpp>
#include "PhysicsObject.h"

//Forward declaration of spatial map to avoid circular dependencies
class SpatialMap;

class Circle : public PhysicsObject{
private:
    //The rendering of the circle
    sf::CircleShape shape;

    float radius;
    sf::Color color;

public:
    //Constructor (PhysicsObject parent)
    Circle(sf::Vector2f& p, float r, float m, sf::Color& color);

    void highlight() override;

    void unhighlight() override;

    void setRadius(float r);

    float getRadius();

    //Virtual checkBounds override
    void checkBounds(sf::FloatRect bounds,float restitution) override;

    //Rendering of circle onto screen
    void draw(sf::RenderWindow& window) override;

    //Collision detection/response with another PhysicsObject depending on coefficient of restitution
    void collide(PhysicsObject& other, float res) override;

    std::unique_ptr<PhysicsObject> clone() const override;
};

#endif