#include "../../Physics/Simulations/Collisions.h"
#include "../Button.h"
#include "../Slider.h"
#include "../Spinner.h"
#include "../Label.h"

void Collisions::initUI(sf::Font& font){
    /** CORE SIM CONTROL BUTTONS */

    Button* startStop = new Button({345.f,5.f},{150.f,50.f},font);
    startStop->setText(std::string("Start/Stop"));
    startStop->setOnClick([this,startStop](){
        if(simulating){
            simulating=false;
            startStop->setText("Start");
        }else{
            simulating=true;
            startStop->setText("Stop");
        }
    });
    UIElements.push_back(std::unique_ptr<Button>(startStop));

    Button* saveState = new Button({345.f,65.f},{150.f,50.f},font);
    saveState->setText(std::string("Save State"));
    saveState->setOnClick([this](){
        //Placeholder for future save state functionality
    });
    UIElements.push_back(std::unique_ptr<Button>(saveState));

    Button* loadState = new Button({345.f,125.f},{150.f,50.f},font);
    loadState->setText(std::string("Load State"));
    loadState->setOnClick([this](){
        //Placeholder for future load state functionality
    });
    UIElements.push_back(std::unique_ptr<Button>(loadState));

    Button* reset = new Button({345.f,185.f},{150.f,50.f},font);
    reset->setText(std::string("Reset"));
    reset->setOnClick([this](){
        objects.clear();
    });
    UIElements.push_back(std::unique_ptr<Button>(reset));


    /** SIMULATION PARAMETER CONTROLS */


    Slider* gravitySlider = new Slider({23.f,70.f},{300.f,40.f}, font,0.f,100.f,gravity/10.f);
    gravitySlider->setOnChange([this,gravitySlider](){
        gravity=10*gravitySlider->getValue();
        gravitySlider->setText("Gravity: " + formatFloatToDecimalPlaces(gravity/10.f,2));
    });
    gravitySlider->runOnChange();
    UIElements.push_back(std::unique_ptr<Slider>(gravitySlider));

    Slider* colRestitutionSlider = new Slider({23.f,140.f},{300.f,40.f}, font,0.f,1.f,colRestitution);
    colRestitutionSlider->setOnChange([this,colRestitutionSlider](){
        colRestitution=colRestitutionSlider->getValue();
        colRestitutionSlider->setText("Collision Restitution: " + formatFloatToDecimalPlaces(colRestitution,2));
    });
    colRestitutionSlider->runOnChange();
    UIElements.push_back(std::unique_ptr<Slider>(colRestitutionSlider));

    Slider* boundsRestitutionSlider = new Slider({23.f,210.f},{300.f,40.f}, font,0.f,1.f,boundsRestitution);
    boundsRestitutionSlider->setOnChange([this,boundsRestitutionSlider](){
        boundsRestitution=boundsRestitutionSlider->getValue();
        boundsRestitutionSlider->setText("Bounds Restitution: " + formatFloatToDecimalPlaces(boundsRestitution,2));
    });
    boundsRestitutionSlider->runOnChange();
    UIElements.push_back(std::unique_ptr<Slider>(boundsRestitutionSlider));

    Slider* cellSizeSlider = new Slider({23.f,280.f},{300.f,40.f}, font,5.f,100.f,(float)sm.getCellSize());
    cellSizeSlider->setOnChange([this,cellSizeSlider](){
        sm.setCellSize(static_cast<int>(cellSizeSlider->getValue()));
        cellSizeSlider->setText("Cell Size: " + std::to_string(static_cast<int>(cellSizeSlider->getValue())));
    });
    cellSizeSlider->runOnChange();
    UIElements.push_back(std::unique_ptr<Slider>(cellSizeSlider));


    /** DETAIL PANEL */

    //Position spinners
    
    Spinner* posXSpinner = new Spinner({10.f,650.f},{120.f,40.f}, font,0.f,simBounds.size.x,0.f);
    posXSpinner->setOnChange([this,posXSpinner](){
        //if selected object exists, update its x position
        if(this->selectedObject!=nullptr){
            this->selectedObject->setPos({
                posXSpinner->getValue()+simBounds.position.x,
                this->selectedObject->getPos().y
            });
        }
        posXSpinner->setText("X [" + formatFloatToDecimalPlaces(posXSpinner->getValue(),1)+"]");
    });
    posXSpinner->setLiveUpdate([this,posXSpinner](){
        //if selected object exists, update spinner text to match its x position
        if(this->selectedObject!=nullptr){
             posXSpinner->setValue(this->selectedObject->getPos().x-simBounds.position.x);
        }
        posXSpinner->setText("X [" + formatFloatToDecimalPlaces(posXSpinner->getValue(),1)+"]");
    });
    posXSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(posXSpinner));

    Spinner* posYSpinner = new Spinner({180.f,650.f},{120.f,40.f}, font,0.f,simBounds.size.y,0.f);
    posYSpinner->setOnChange([this,posYSpinner](){
        //if selected object exists, update its y position
        if(this->selectedObject!=nullptr){
            this->selectedObject->setPos({
                this->selectedObject->getPos().x,
                simBounds.size.y-posYSpinner->getValue()+simBounds.position.y
            });
        }
        posYSpinner->setText("Y [" + formatFloatToDecimalPlaces(posYSpinner->getValue(),1)+"]");
    });
    posYSpinner->setLiveUpdate([this,posYSpinner](){
        //if selected object exists, update spinner text to match its y position
        if(this->selectedObject!=nullptr){
             posYSpinner->setValue(simBounds.size.y-this->selectedObject->getPos().y+simBounds.position.y);
        }
        posYSpinner->setText("Y [" + formatFloatToDecimalPlaces(posYSpinner->getValue(),1)+"]");
    });
    posYSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(posYSpinner));

    //velocity spinners
    Spinner* velXSpinner = new Spinner({10.f,750.f},{120.f,40.f}, font,-FLT_MAX,FLT_MAX,0.f);
    velXSpinner->setOnChange([this,velXSpinner](){
        //if selected object exists, update its x position
        if(this->selectedObject!=nullptr){
            this->selectedObject->setVel({
                velXSpinner->getValue(),
                this->selectedObject->getVel().y
            });
        }
        velXSpinner->setText("X [" + formatFloatToDecimalPlaces(velXSpinner->getValue(),1)+"]");
    });
    velXSpinner->setLiveUpdate([this,velXSpinner](){
        //if selected object exists, update spinner text to match its x position
        if(this->selectedObject!=nullptr){
             velXSpinner->setValue(this->selectedObject->getVel().x);
        }
        velXSpinner->setText("X [" + formatFloatToDecimalPlaces(velXSpinner->getValue(),1)+"]");
    });
    velXSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(velXSpinner));

    Spinner* velYSpinner = new Spinner({180.f,750.f},{120.f,40.f}, font,-FLT_MAX,FLT_MAX,0.f);
    velYSpinner->setOnChange([this,velYSpinner](){
        //if selected object exists, update its y position
        if(this->selectedObject!=nullptr){
            this->selectedObject->setVel({
                this->selectedObject->getVel().x,
                -velYSpinner->getValue()
            });
        }
        velYSpinner->setText("Y [" + formatFloatToDecimalPlaces(velYSpinner->getValue(),1)+"]");
    });
    velYSpinner->setLiveUpdate([this,velYSpinner](){
        //if selected object exists, update spinner text to match its y position
        if(this->selectedObject!=nullptr){
             velYSpinner->setValue(-this->selectedObject->getVel().y);
        }
        velYSpinner->setText("Y [" + formatFloatToDecimalPlaces(velYSpinner->getValue(),1)+"]");
    });
    velYSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(velYSpinner));
    
}