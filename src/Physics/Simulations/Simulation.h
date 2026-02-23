#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include "../Systems.h"
#include <functional>
#include "../../UI/UI.h"
#include <iomanip>
#include <sstream>

#include "../PhysicsState.h"

struct SaveState{
    PhysicsState savedState;
    std::vector<std::unique_ptr<sf::Shape>> savedObjectShapes;
    std::vector<std::unique_ptr<UI>> savedUIElements;
    float savedTimeElapsed;
};

enum class SimType{
    Menu,
    Collisions,
    Gravity,
    EM
};

struct SpawnParams{
    float mass;
    float vx;
    float vy;
};


class Simulation{
    public:
         //Collection of physics objects in the simulation
        PhysicsState state;
        //Collection of shapes corresponding to physics objects, used for drawing. Index of shape in this vector corresponds to index of physics object in physicsState
        std::vector<std::unique_ptr<sf::Shape>> objectShapes;
        //Collection of UI elements
        std::vector<std::unique_ptr<UI>> UIElements;

        //a save state
        SaveState save;

        //Selected object for detail panel
        int selectedObjectIndex=-1; //index of selected object in physicsState, -1 if no object is selected

        //Spawning parameters for new objects
        SpawnParams params;

        bool simulating=false;
        
        float scaleFactor=100.f; //100 pixels = 1 meter
        float timeFactor=1.f; //can be used to speed up or slow down the simulation
        float timeElapsed=0.f; //amount of time elapsed in sim


        virtual void update(float dt)=0;
        virtual void draw(sf::RenderWindow& window)=0;
        virtual void initUI(sf::Font& font)=0;
        virtual void drawUI(sf::RenderWindow& window)=0;
        virtual void handleEvent(const sf::Event& event)=0;
        virtual int getPopulation()=0;
        virtual ~Simulation()=default;
};


inline std::string formatFloatToSigFigs(float value, int sigfigs){
    //formats a float to specified # of significant figures
    std::stringstream stream;
    stream << std::setprecision(sigfigs) << value;
    return stream.str();
}

#endif