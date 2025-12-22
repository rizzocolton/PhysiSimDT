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
    PhysicsObject(sf::Vector2f& p, float m);

    //Physics

    void update(float dt);
    void push(sf::Vector2f& force,float dt);

    //Accessors

    sf::Vector2f getPos();
    sf::Vector2f getVel();
    float getMass();

    //Mutators

    void setPos(sf::Vector2f p);
    void setVel(sf::Vector2f v);
    void setMass(float m);

    //Virtuals

    //Subclasses (different shaped objects) may check to see if their projection is within bounds (the screen in which they are enclosed)
    virtual void checkBounds(sf::FloatRect bounds,float restitution)=0;
    //Subclasses (different shaped objects) utilize their own draw implementation according to their geometry
    virtual void draw(sf::RenderWindow& window)=0;
    //Subclasses (different shaped objects) utilize their own collision detection and response according to their geometry
    virtual void collide(PhysicsObject& other, float restitution)=0;
    //Subclasses should be able to highlight themselves when selected
    virtual void highlight()=0;
    //Should also unhighlight themselves
    virtual void unhighlight()=0;

    //for copying into save sates
    virtual std::unique_ptr<PhysicsObject> clone() const=0;

    virtual ~PhysicsObject() = default;
    
};

#endif