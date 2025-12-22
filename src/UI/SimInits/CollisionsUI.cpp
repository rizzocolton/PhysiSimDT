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


    Slider* gravitySlider = new Slider({23.f,70.f},{300.f,40.f}, font,0.f,100.f,gravity);
    gravitySlider->setOnChange([this,gravitySlider](){
        gravity=gravitySlider->getValue();
        gravitySlider->setText("Gravity: " + formatFloatToDecimalPlaces(gravity,2));
    });
    gravitySlider->runOnChange();
    UIElements.push_back(std::unique_ptr<Slider>(gravitySlider));

    Slider* colRestitutionSlider = new Slider({23.f,140.f},{300.f,40.f}, font,0.f,1.f,colRestitution);
    colRestitutionSlider->setOnChange([this,colRestitutionSlider](){
        colRestitution=colRestitutionSlider->getValue();
        colRestitutionSlider->setText("Collision Restitution: " + formatFloatToDecimalPlaces(colRestitution,1));
    });
    colRestitutionSlider->runOnChange();
    UIElements.push_back(std::unique_ptr<Slider>(colRestitutionSlider));

    Slider* boundsRestitutionSlider = new Slider({23.f,210.f},{300.f,40.f}, font,0.f,1.f,boundsRestitution);
    boundsRestitutionSlider->setOnChange([this,boundsRestitutionSlider](){
        boundsRestitution=boundsRestitutionSlider->getValue();
        boundsRestitutionSlider->setText("Bounds Restitution: " + formatFloatToDecimalPlaces(boundsRestitution,1));
    });
    boundsRestitutionSlider->runOnChange();
    UIElements.push_back(std::unique_ptr<Slider>(boundsRestitutionSlider));

    Slider* cellSizeSlider = new Slider({23.f,280.f},{300.f,40.f}, font,0.01f,10.f,(float)sm.getCellSize()/scaleFactor);
    cellSizeSlider->setOnChange([this,cellSizeSlider](){
        sm.setCellSize(static_cast<int>(cellSizeSlider->getValue()*scaleFactor));
        cellSizeSlider->setText("Cell Size: " + formatFloatToDecimalPlaces(cellSizeSlider->getValue(),2));
    });
    cellSizeSlider->runOnChange();
    UIElements.push_back(std::unique_ptr<Slider>(cellSizeSlider));

    Slider* simSpeedSlider = new Slider({23.f,350.f},{300.f,40.f},font,0.01f,10.f,1.f);
    simSpeedSlider->setOnChange([this,simSpeedSlider](){
        this->timeFactor=simSpeedSlider->getValue();
        simSpeedSlider->setText("Sim Speed: "+formatFloatToDecimalPlaces(simSpeedSlider->getValue(),2));
    });
    simSpeedSlider->runOnChange();
    UIElements.push_back(std::unique_ptr<Slider>(simSpeedSlider));


    /** DETAIL PANEL */

    //Position spinners
    
    Spinner* posXSpinner = new Spinner({10.f,650.f},{120.f,40.f}, font,0.f,simBounds.size.x/scaleFactor,0.f);
    posXSpinner->setOnChange([this,posXSpinner](){
        //if selected object exists, update its x position
        if(this->selectedObject!=nullptr){
            
            Circle* circle = dynamic_cast<Circle*>(this->selectedObject);
            if(circle!=nullptr){
                posXSpinner->setRange(circle->getRadius()/scaleFactor,simBounds.size.x/scaleFactor);
            }

            this->selectedObject->setPos({
                posXSpinner->getValue()*scaleFactor+simBounds.position.x,
                this->selectedObject->getPos().y
            });
        }
        posXSpinner->setText("X [" + formatFloatToDecimalPlaces(posXSpinner->getValue(),2)+"]");
    });
    posXSpinner->setLiveUpdate([this,posXSpinner](){
        //if selected object exists, update spinner text to match its x position
        if(this->selectedObject!=nullptr){
             posXSpinner->setValue((this->selectedObject->getPos().x-simBounds.position.x)/scaleFactor);
        }
        posXSpinner->setText("X [" + formatFloatToDecimalPlaces(posXSpinner->getValue(),2)+"]");
    });
    posXSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(posXSpinner));

    Spinner* posYSpinner = new Spinner({170.f,650.f},{120.f,40.f}, font,0.f,simBounds.size.y/scaleFactor,0.f);
    posYSpinner->setOnChange([this,posYSpinner](){
        //if selected object exists, update its y position
        if(this->selectedObject!=nullptr){

            Circle* circle = dynamic_cast<Circle*>(this->selectedObject);
            if(circle!=nullptr){
                posYSpinner->setRange(circle->getRadius()/scaleFactor,simBounds.size.y/scaleFactor);
            }

            this->selectedObject->setPos({
                this->selectedObject->getPos().x,
                simBounds.size.y-posYSpinner->getValue()*scaleFactor+simBounds.position.y
            });
        }
        posYSpinner->setText("Y [" + formatFloatToDecimalPlaces(posYSpinner->getValue(),2)+"]");
    });
    posYSpinner->setLiveUpdate([this,posYSpinner](){
        //if selected object exists, update spinner text to match its y position
        if(this->selectedObject!=nullptr){
             posYSpinner->setValue((simBounds.size.y-this->selectedObject->getPos().y+simBounds.position.y)/scaleFactor);
        }
        posYSpinner->setText("Y [" + formatFloatToDecimalPlaces(posYSpinner->getValue(),2)+"]");
    });
    posYSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(posYSpinner));

    //velocity spinners
    Spinner* velXSpinner = new Spinner({10.f,750.f},{120.f,40.f}, font,-FLT_MAX,FLT_MAX,0.f);
    velXSpinner->setOnChange([this,velXSpinner](){
        //if selected object exists, update its x position
        if(this->selectedObject!=nullptr){
            this->selectedObject->setVel({
                velXSpinner->getValue()*scaleFactor,
                this->selectedObject->getVel().y
            });
        }
        velXSpinner->setText("X [" + formatFloatToDecimalPlaces(velXSpinner->getValue(),1)+"]");
    });
    velXSpinner->setLiveUpdate([this,velXSpinner](){
        //if selected object exists, update spinner text to match its x position
        if(this->selectedObject!=nullptr){
             velXSpinner->setValue(this->selectedObject->getVel().x/scaleFactor);
        }
        velXSpinner->setText("X [" + formatFloatToDecimalPlaces(velXSpinner->getValue(),2)+"]");
    });
    velXSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(velXSpinner));

    Spinner* velYSpinner = new Spinner({170.f,750.f},{120.f,40.f}, font,-FLT_MAX,FLT_MAX,0.f);
    velYSpinner->setOnChange([this,velYSpinner](){
        //if selected object exists, update its y position
        if(this->selectedObject!=nullptr){
            this->selectedObject->setVel({
                this->selectedObject->getVel().x,
                -velYSpinner->getValue()*scaleFactor
            });
        }
        velYSpinner->setText("Y [" + formatFloatToDecimalPlaces(velYSpinner->getValue(),2)+"]");
    });
    velYSpinner->setLiveUpdate([this,velYSpinner](){
        //if selected object exists, update spinner text to match its y position
        if(this->selectedObject!=nullptr){
             velYSpinner->setValue(-this->selectedObject->getVel().y/scaleFactor);
        }
        velYSpinner->setText("Y [" + formatFloatToDecimalPlaces(velYSpinner->getValue(),2)+"]");
    });
    velYSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(velYSpinner));

    //mass spinner

    Spinner* massSpinner = new Spinner({50.f,800.f},{120.f,40.f}, font,0.1f,FLT_MAX,0.f);
    massSpinner->setOnChange([this,massSpinner](){
        //if selected object exists, update its mass
        if(this->selectedObject!=nullptr){
            this->selectedObject->setMass(massSpinner->getValue());
        }
        massSpinner->setText("Mass [" + formatFloatToDecimalPlaces(massSpinner->getValue(),2)+"]");
    });
    massSpinner->setLiveUpdate([this, massSpinner](){
        //if selected object exists, make the spinner match its mass
        if(this->selectedObject!=nullptr){
            massSpinner->setValue(this->selectedObject->getMass());
        }
        massSpinner->setText("Mass [" + formatFloatToDecimalPlaces(massSpinner->getValue(),2)+"]");
    });
    massSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(massSpinner));

    //radius spinner (for circles)
    Spinner* radiusSpinner = new Spinner({50.f,850.f},{120.f,40.f}, font,1.f/scaleFactor,FLT_MAX,0.f);
    radiusSpinner->setOnChange([this,radiusSpinner](){
        //if selected object exists and is a circle, update its radius
        if(this->selectedObject!=nullptr){
            Circle* circle = dynamic_cast<Circle*>(this->selectedObject);
            if(circle!=nullptr){
                circle->setRadius(radiusSpinner->getValue()*scaleFactor);
            }
        }
        radiusSpinner->setText("Radius [" + formatFloatToDecimalPlaces(radiusSpinner->getValue(),2)+"]");
    });
    radiusSpinner->setLiveUpdate([this,radiusSpinner](){
        //if selected object exists and is a circle, update spinner to match its radius
        if(this->selectedObject!=nullptr){
            Circle* circle = dynamic_cast<Circle*>(this->selectedObject);
            if(circle!=nullptr){
                radiusSpinner->setRange(1.f/scaleFactor,this->sm.getCellSize()/(scaleFactor));
                radiusSpinner->setValue(circle->getRadius()/scaleFactor);
            }
        }
        radiusSpinner->setText("Radius [" + formatFloatToDecimalPlaces(radiusSpinner->getValue(),2)+"]");
    });
    radiusSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(radiusSpinner));
    
}