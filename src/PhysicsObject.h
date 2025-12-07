#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <SFML/Graphics.hpp>

class PhysicsObject{
protected:
    //x,y position
    sf::Vector2f pos;
    //x,y velocity
    sf::Vector2f vel;
    float mass;

public:
    //Constructor
    PhysicsObject(sf::Vector2f p, float m);

    //Physics

    void update(float dt);
    void push(sf::Vector2f force,float dt);

    //Accessors

    sf::Vector2f getPos();
    sf::Vector2f getVel();
    float getMass();

    //Mutators

    void setPos(sf::Vector2f p);
    void setVel(sf::Vector2f v);
    void setMass(float m);

    //Virtuals

    //Subclasses (different shaped objects) may check to see if their projection is within bounds
    virtual void checkBounds()=0;
    //Subclasses (different shaped objects) utilize their own draw implementation according to their geometry
    virtual void draw(sf::RenderWindow& window)=0;

};

#endif