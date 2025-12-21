#include "Circle.h"
#include "../SpatialMap.h"
#include <iostream>

//Constructor

Circle::Circle(sf::Vector2f& p, float r, float m, sf::Color& c):
PhysicsObject(p,m){
    radius=r;
    color=c;
    shape.setRadius(radius);
    shape.setPosition(pos);
    //makes origin of circle at its center, much easier/intuitive for collisions
    shape.setOrigin({radius,radius});
    shape.setFillColor(color);
};

void Circle::highlight(){
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(-2.f);
}

void Circle::unhighlight(){
    shape.setOutlineThickness(0.f);
}

void Circle::setRadius(float r){
    radius=r;
    shape.setRadius(radius);
    shape.setOrigin({radius,radius});
}

float Circle::getRadius(){
    return radius;
}

//overrides virtual check boundaries for circular geometry
void Circle::checkBounds(sf::FloatRect bounds,float restitution){
    //if the circle is outside of the sim and actively moving outside of the sim, reverse the component moving away
    if((vel.x<0&&pos.x-radius<=bounds.position.x)||(vel.x>0&&pos.x+radius>=bounds.position.x+bounds.size.x)){
        vel.x*=-1*restitution;
    }

    if((vel.y<0&&pos.y-radius<=bounds.position.y)||(vel.y>0&&pos.y+radius>=bounds.position.y+bounds.size.y)){
        vel.y*=-1*restitution;
        //prevents clipping due to gravity when near the bottom boundary
        if(std::abs(vel.y)<0.1f){
            vel.y=0.f;
        }
    }
}

//renders the circle onto selected window
void Circle::draw(sf::RenderWindow& window){
    shape.setPosition(pos);
    window.draw(shape);
}



//simple collision detection/response with another PhysicsObject
void Circle::collide(PhysicsObject& other, float res){
    //don't collide with self
    Circle* otherCircle = dynamic_cast<Circle*>(&other);
    if(otherCircle==this) return; 

    //don't collide if not intersecting or if in the exact same position
    sf::Vector2f otherPos=otherCircle->getPos();
    sf::Vector2f diff=otherPos-pos;
    if(diff.lengthSquared()>(radius+otherCircle->radius)*(radius+otherCircle->radius)||diff.lengthSquared()<0.001f) return; 

    

    /*
    if intersecting push back so no longer intersecting
    sf::Vector2f correction=(radius+other.radius-diff.length())*diff.normalized()/2.0f;
    pos-=correction;
    other.pos+=correction;
    */
    
    //^I'm not going to do the one above as it will result in inaccurate positions.
    
    //COLLISION CALCULATION

    //component of this circles velocity along the line of impact
    sf::Vector2f v1_impact=vel.projectedOnto(diff);
    //component of this circles velocity along the plane of contact
    sf::Vector2f v1_contact=vel.projectedOnto(diff.perpendicular());
    //component of other circles velocity along the line of impact
    sf::Vector2f v2_impact=otherCircle->vel.projectedOnto(diff);
    //component of other circles velocity along the plane of contact
    sf::Vector2f v2_contact=otherCircle->vel.projectedOnto(diff.perpendicular());

    //don't collide if moving apart
    if((v1_impact-v2_impact).dot(diff.normalized())<=0) return;

    //computes resultant velocity for this circle along the line of impact (justification/explanation in docs)
    sf::Vector2f v1_impact_prime=((mass-res*otherCircle->mass)*v1_impact+
                (1+res)*v2_impact*otherCircle->mass)/
                (mass+otherCircle->mass);
    
    //computes resultant velocity for othre circle along the line of impact
    sf::Vector2f v2_impact_prime=((otherCircle->mass-res*mass)*v2_impact+
                (1+res)*v1_impact*mass)/
                (mass+otherCircle->mass);

    //reconstructs and assigns new velocity vectors to respective circles
    vel=v1_impact_prime+v1_contact;
    otherCircle->vel=v2_impact_prime+v2_contact;
}