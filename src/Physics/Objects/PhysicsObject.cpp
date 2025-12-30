#include "PhysicsObject.h"


//Constructor

PhysicsObject::PhysicsObject(sf::Vector2f& p, float m){
    pos=p;
    mass=m;
    charge=0.f;
    vel={0.f,0.f};
}

PhysicsObject::PhysicsObject(sf::Vector2f& p, float m, float c){
    pos=p;
    mass=m;
    charge=c;
    vel={0.f,0.f};
}

//Physics

//Changes position by velocity times time elapsed. Then checks to see if object is still in bounds.
void PhysicsObject::move(float dt){
    pos+=vel*dt;
}

//"pushes" object by a force times time elapsed. this impulse is then divided by mass of the object to create a change in velocity
void PhysicsObject::push(sf::Vector2f& force, float dt){
    vel+=(force/mass)*dt;
}

//Accessors

sf::Vector2f PhysicsObject::getPos(){
    return pos;
}

sf::Vector2f PhysicsObject::getVel(){
    return vel;
}

float PhysicsObject::getMass(){
    return mass;
}

float PhysicsObject::getCharge(){
    return charge;
}

//Mutators

void PhysicsObject::setPos(sf::Vector2f p){
    pos=p;
}

void PhysicsObject::setVel(sf::Vector2f v){
    vel=v;
}

void PhysicsObject::setMass(float m){
    mass=m;
}

void PhysicsObject::setCharge(float c){
    charge=c;
}
