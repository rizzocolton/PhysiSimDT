#include "Circle.h"
#include "Constants.h"

//Constructor

Circle::Circle(sf::Vector2f p, float r, float m, sf::Color c):
PhysicsObject(p,m){
    radius=r;
    mass=m;
    pos=p;
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