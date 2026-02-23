#include "../../Physics/Simulations/Collisions.h"
#include "../Button.h"
#include "../Slider.h"
#include "../Spinner.h"
#include "../Label.h"
#include <iostream>
void Collisions::initUI(sf::Font& font){
    /** CORE SIM CONTROL BUTTONS */

    Button* exitSim = new Button({5.f,5.f},{50.f,50.f},font);
    exitSim->setText("<");
    //need to capture switching function by value instead of reference, otherwise the lambda may delete itself!
    exitSim->setOnChange([sm=this->switchSim,exitSim](){
        sm(SimType::Menu);
    });
    UIElements.push_back(std::unique_ptr<Button>(exitSim));

    Button* startStop = new Button({345.f,5.f},{150.f,50.f},font);
    startStop->setText(std::string("Start/Stop"));
    startStop->setOnChange([this,startStop](){
        if(simulating){
            simulating=false;
            startStop->setText("Start");
        }else{
            simulating=true;
            startStop->setText("Stop");
        }
    });
    startStop->setLiveUpdate([this,startStop](){
        if(simulating){
            startStop->setText("Stop");
        }else{
            startStop->setText("Start");
        }
    });
    UIElements.push_back(std::unique_ptr<Button>(startStop));

    

    
}