#include "Circle.h"
#include "Constants.h"

//Constructor

Circle::Circle(sf::Vector2f p, float r, float m, sf::Color c):
PhysicsObject(p,m){
    radius=r;
    color=c;
    shape.setRadius(radius);
    shape.setPosition(pos);
    shape.setFillColor(color);
};

//overrides virtual check boundaries for circular geometry
//origin for circle is top left, hence the odd looking inequaliti
void Circle::checkBounds(){
    if(pos.x<0||pos.x+2*radius>SCREEN_WIDTH){
        vel.x*=-1;
    }
    if(pos.y<0||pos.y+2*radius>SCREEN_HEIGHT){
        vel.y*=-1;
    }
}

//renders the circle onto selected window
void Circle::draw(sf::RenderWindow& window){
    shape.setPosition(pos);
    window.draw(shape);
}

//simple collision detection/response with another PhysicsObject
void Circle::collide(Circle& other){
    //don't collide with self
    if(&other==this) return; 

    //don't collide if not intersecting
    sf::Vector2f otherPos=other.getPos();
    sf::Vector2f diff=otherPos-pos;
    if(diff.lengthSquared()>(radius+other.radius)*(radius+other.radius)) return; 
    
    //COLLISION CALCULATION
    
    //unit vector pointing to line of impact
    sf::Vector2f dirOfCollision=diff.normalized();

    //component of this circles velocity along the line of impact
    sf::Vector2f v1_impact=dirOfCollision*(vel.dot(dirOfCollision));
    //component of this circles velocity along the plane of contact
    sf::Vector2f v1_contact=dirOfCollision*vel.cross(dirOfCollision);
    //component of other circles velocity along the line of impact
    sf::Vector2f v2_impact=dirOfCollision*other.vel.dot(dirOfCollision);
    //component of other circles velocity along the plane of contact
    sf::Vector2f v2_contact=dirOfCollision*other.vel.cross(dirOfCollision);

    //computes resultant velocity for this circle along the line of impact (justification/explanation in docs)
    sf::Vector2f v1_impact_prime=((mass+COEFFICIENT_OF_RESTITUTION*other.mass)*v1_impact+
                (other.mass-COEFFICIENT_OF_RESTITUTION*other.mass)*v2_impact)/
                (mass+other.mass);
    
    //computes resultant velocity for othre circle along the line of impact
    sf::Vector2f v2_impact_prime=COEFFICIENT_OF_RESTITUTION*(v2_impact-v1_impact)+v1_impact_prime;

    //reconstructs and assigns new velocity vectors to respective circles
    vel=v1_impact_prime+v1_contact;
    other.vel=v2_impact_prime+v2_contact;
}