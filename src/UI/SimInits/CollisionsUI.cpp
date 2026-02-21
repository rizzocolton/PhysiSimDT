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

    Button* saveState = new Button({345.f,65.f},{150.f,50.f},font);
    saveState->setText(std::string("Save State"));
    saveState->setOnChange([this](){
        this->save.savedObjects.clear();
        for(auto& obj: this->objects){
            this->save.savedObjects.push_back(obj->clone());
        }
        this->save.savedTimeElapsed=timeElapsed;
    });
    UIElements.push_back(std::unique_ptr<Button>(saveState));

    Button* loadState = new Button({345.f,125.f},{150.f,50.f},font);
    loadState->setText(std::string("Load State"));
    loadState->setOnChange([this](){
        this->selectedObject=nullptr;
        this->objects.clear();
        for(auto& obj: this->save.savedObjects){
            this->objects.push_back(obj->clone());
            this->sm.enterCell(this->objects.back().get(),scaleFactor);
        }
        timeElapsed=this->save.savedTimeElapsed;
    });
    UIElements.push_back(std::unique_ptr<Button>(loadState));

    Button* reset = new Button({345.f,185.f},{150.f,50.f},font);
    reset->setText(std::string("Reset"));
    reset->setOnChange([this,&font](){
        selectedObject=nullptr;
        timeElapsed=0;
        objects.clear();
    });
    UIElements.push_back(std::unique_ptr<Button>(reset));


    /** SIMULATION PARAMETER CONTROLS */


    Spinner* gravitySpinner = new Spinner({10.f,70.f},{300.f,40.f}, font,0.f,100.f,gravity);
    gravitySpinner->setOnChange([this,gravitySpinner](){
        gravity = gravitySpinner->getValue();
        gravitySpinner->setText("Gravity: " + formatFloatToSigFigs(gravity,4));
    });
    gravitySpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(gravitySpinner));

    Spinner* colRestitutionSpinner = new Spinner({10.f,140.f},{300.f,40.f}, font,0.f,1.f,colRestitution);
    colRestitutionSpinner->setOnChange([this,colRestitutionSpinner](){
        colRestitution = colRestitutionSpinner->getValue();
        colRestitutionSpinner->setText("Collision Restitution: " + formatFloatToSigFigs(colRestitution,4));
    });
    colRestitutionSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(colRestitutionSpinner));

    Spinner* boundsRestitutionSpinner = new Spinner({10.f,210.f},{300.f,40.f}, font,0.f,1.f,boundsRestitution);
    boundsRestitutionSpinner->setOnChange([this,boundsRestitutionSpinner](){
        boundsRestitution = boundsRestitutionSpinner->getValue();
        boundsRestitutionSpinner->setText("Bounds Restitution: " + formatFloatToSigFigs(boundsRestitution,4));
    });
    boundsRestitutionSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(boundsRestitutionSpinner));

    Spinner* cellSizeSpinner = new Spinner({10.f,280.f},{300.f,40.f}, font,1/scaleFactor,simBounds.size.y,(float)sm.getCellSize()/scaleFactor);
    cellSizeSpinner->setOnChange([this,cellSizeSpinner](){
        //cell size cannot be smaller than 2x largest radius or objects may fail to be placed in the correct cell, causing tunneling issues
        float maxRadius=0.f;
        for(auto& obj: objects){
            Circle* circle = dynamic_cast<Circle*>(obj.get());
            if(circle!=nullptr){
                maxRadius=std::max(maxRadius,circle->getRadius());
            }
        }
        cellSizeSpinner->setRange(std::max(1/scaleFactor,maxRadius*2),simBounds.size.y);
        //need to convert to pixels for spatial map, but still displaying in sim units for user
        sm.setCellSize(static_cast<int>(cellSizeSpinner->getValue()*scaleFactor));
        cellSizeSpinner->setText("Cell Size: " + formatFloatToSigFigs(cellSizeSpinner->getValue(),3));
    });
    cellSizeSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(cellSizeSpinner));

    Spinner* simSpeedSpinner = new Spinner({10.f,350.f},{300.f,40.f},font,0.01f,10.f,1.f);
    simSpeedSpinner->setOnChange([this,simSpeedSpinner](){
        this->timeFactor = simSpeedSpinner->getValue();
        simSpeedSpinner->setText("Sim Speed: "+formatFloatToSigFigs(simSpeedSpinner->getValue(),3));
    });
    simSpeedSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(simSpeedSpinner));

    Spinner* scaleFactorSpinner = new Spinner({10.f,420.f},{300.f,40.f},font,1.f,500.f,100.f);
    scaleFactorSpinner->setOnChange([this,scaleFactorSpinner](){
        //adjusting sim bounds to keep the same physical size in pixels
        simBounds.size.x=simBounds.size.x*(scaleFactor/scaleFactorSpinner->getValue());
        simBounds.size.y=simBounds.size.y*(scaleFactor/scaleFactorSpinner->getValue());
        simBounds.position.x=simBounds.position.x*(scaleFactor/scaleFactorSpinner->getValue());
        simBounds.position.y=simBounds.position.y*(scaleFactor/scaleFactorSpinner->getValue());
        
        this->scaleFactor = scaleFactorSpinner->getValue();
        scaleFactorSpinner->setText("Scale (ppm): "+formatFloatToSigFigs(scaleFactorSpinner->getValue(),3));
    });
    scaleFactorSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(scaleFactorSpinner));

    Button* showGridButton=new Button({10.f,490.f},{300.f,40.f},font);
    showGridButton->setText("Show Grid: " + std::string(showGrid?"On":"Off"));
    showGridButton->setOnChange([this,showGridButton](){
        this->showGrid=!this->showGrid;
        showGridButton->setText("Show Grid: " + std::string(showGrid?"On":"Off"));
    });
    UIElements.push_back(std::unique_ptr<Button>(showGridButton));


    /** DETAIL PANEL */

    //Position spinners
    
    Spinner* posXSpinner = new Spinner({10.f,650.f},{120.f,40.f},font,0.f,simBounds.size.x,0.f);
    posXSpinner->setOnChange([this,posXSpinner](){
        //if selected object exists, update its x position
        if(this->selectedObject!=nullptr){
            
            Circle* circle = dynamic_cast<Circle*>(this->selectedObject);
            if(circle!=nullptr){
                posXSpinner->setRange(circle->getRadius(),simBounds.size.x-circle->getRadius());
            }

            this->selectedObject->setPos({
                posXSpinner->getValue(),
                this->selectedObject->getPos().y
            });
        }
        posXSpinner->setText("X [" + formatFloatToSigFigs(posXSpinner->getValue(),3)+"] m");
    });
    posXSpinner->setLiveUpdate([this,posXSpinner](){
        //if selected object exists, update spinner text to match its x position
        if(this->selectedObject!=nullptr){
             posXSpinner->setValue(this->selectedObject->getPos().x);
        }
        posXSpinner->setText("X [" + formatFloatToSigFigs(posXSpinner->getValue(),3)+"] m");
    });
    posXSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(posXSpinner));

    Spinner* posYSpinner = new Spinner({170.f,650.f},{120.f,40.f},font,0.f,simBounds.size.y,0.f);
    posYSpinner->setOnChange([this,posYSpinner](){
        //if selected object exists, update its y position
        if(this->selectedObject!=nullptr){
            
            Circle* circle = dynamic_cast<Circle*>(this->selectedObject);
            if(circle!=nullptr){
                posYSpinner->setRange(circle->getRadius(),simBounds.size.y-circle->getRadius());
            }

            this->selectedObject->setPos({
                this->selectedObject->getPos().x,
                simBounds.size.y-posYSpinner->getValue()
            });
        }
        posYSpinner->setText("Y [" + formatFloatToSigFigs(posYSpinner->getValue(),3)+"] m");
    });
    posYSpinner->setLiveUpdate([this,posYSpinner](){
        //if selected object exists, update spinner text to match its y position
        if(this->selectedObject!=nullptr){
             posYSpinner->setValue(simBounds.size.y-this->selectedObject->getPos().y);
        }
        posYSpinner->setText("Y [" + formatFloatToSigFigs(posYSpinner->getValue(),3)+"] m");
    });
    posYSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(posYSpinner));   

    //velocity spinners
    Spinner* velXSpinner = new Spinner({10.f,750.f},{120.f,40.f}, font,-FLT_MAX,FLT_MAX,0.f);
    velXSpinner->setOnChange([this,velXSpinner](){
        //if selected object exists, update its x velocity (meters per second)
        if(this->selectedObject!=nullptr){
            this->selectedObject->setVel({
                velXSpinner->getValue(),
                this->selectedObject->getVel().y
            });
        }else{
            //if no object selected update spawning params
            params.vx=velXSpinner->getValue();
        }
        velXSpinner->setText("X [" + formatFloatToSigFigs(velXSpinner->getValue(),3)+"] m/s");
    });
    velXSpinner->setLiveUpdate([this,velXSpinner](){
        if(this->selectedObject!=nullptr){
             velXSpinner->setValue(this->selectedObject->getVel().x);
        }
        velXSpinner->setText("X [" + formatFloatToSigFigs(velXSpinner->getValue(),3)+"] m/s");
    });
    velXSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(velXSpinner));

    Spinner* velYSpinner = new Spinner({170.f,750.f},{120.f,40.f}, font,-FLT_MAX,FLT_MAX,0.f);
    velYSpinner->setOnChange([this,velYSpinner](){
        //if selected object exists, update its y velocity (meters per second)
        if(this->selectedObject!=nullptr){
            this->selectedObject->setVel({
                this->selectedObject->getVel().x,
                -velYSpinner->getValue()
            });
        }else{
            //if no object selected update spawning params
            params.vy=-velYSpinner->getValue();
        }
        velYSpinner->setText("Y [" + formatFloatToSigFigs(velYSpinner->getValue(),3)+"] m/s");
    });
    velYSpinner->setLiveUpdate([this,velYSpinner](){
        //if selected object exists, update spinner text to match its y position
        if(this->selectedObject!=nullptr){
             velYSpinner->setValue(-this->selectedObject->getVel().y);
        }
        velYSpinner->setText("Y [" + formatFloatToSigFigs(velYSpinner->getValue(),3)+"] m/s");
    });
    velYSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(velYSpinner));

    //mass spinner

    Spinner* massSpinner = new Spinner({50.f,800.f},{120.f,40.f}, font,0.1f,FLT_MAX,0.f);
    massSpinner->setOnChange([this,massSpinner](){
        //if selected object exists, update its mass
        if(this->selectedObject!=nullptr){
            this->selectedObject->setMass(massSpinner->getValue());
        }else{
            //if no object selected update spawning params
            params.mass=massSpinner->getValue();
        }
        massSpinner->setText("Mass [" + formatFloatToSigFigs(massSpinner->getValue(),3)+"] kg");
    });
    massSpinner->setLiveUpdate([this, massSpinner](){
        //if selected object exists, make the spinner match its mass
        if(this->selectedObject!=nullptr){
            massSpinner->setValue(this->selectedObject->getMass());
        }
        massSpinner->setText("Mass [" + formatFloatToSigFigs(massSpinner->getValue(),3)+"] kg");
    });
    massSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(massSpinner));

    //radius spinner (for circles), ranges from 1 pixel to half the cell size (to prevent excessively large circles that could cause clipping)
    Spinner* radiusSpinner = new Spinner({50.f,850.f},{120.f,40.f}, font,1/scaleFactor,sm.getCellSize()/(2*scaleFactor),0.1f);
    radiusSpinner->setOnChange([this,radiusSpinner](){
        //if selected object exists and is a circle, update its radius
        if(this->selectedObject!=nullptr){
            Circle* circle = dynamic_cast<Circle*>(this->selectedObject);
            if(circle!=nullptr){
                circle->setRadius(radiusSpinner->getValue());
            }
        }else{
            //if no object selected update spawning params
            params.radius=radiusSpinner->getValue();
        }
        radiusSpinner->setText("Radius [" + formatFloatToSigFigs(radiusSpinner->getValue(),3)+"] m");
    });
    radiusSpinner->setLiveUpdate([this,radiusSpinner](){
        //if selected object exists and is a circle, update spinner to match its radius
        if(this->selectedObject!=nullptr){
            Circle* circle = dynamic_cast<Circle*>(this->selectedObject);
            if(circle!=nullptr){
                radiusSpinner->setRange(1/scaleFactor,this->sm.getCellSize()/(2*scaleFactor));
                radiusSpinner->setValue(circle->getRadius());
            }
        }
        radiusSpinner->setText("Radius [" + formatFloatToSigFigs(radiusSpinner->getValue(),3)+"] m");
    });
    radiusSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(radiusSpinner));

    Label* mechEnergyLabel = new Label({20.f,900.f},font);
    mechEnergyLabel->setLiveUpdate([this,mechEnergyLabel](){
        if(this->selectedObject!=nullptr){
            //KE = 0.5 * m * v^2  (v in m/s, mass in kg)
            float KE = 0.5f * this->selectedObject->getMass() * this->selectedObject->getVel().lengthSquared();
            //U = m * g * h  (h measured in meters from bottom of sim)
            float U = this->selectedObject->getMass() * this->gravity *
                      (this->simBounds.position.y + this->simBounds.size.y - this->selectedObject->getPos().y);
            mechEnergyLabel->setText("ME=" + formatFloatToSigFigs(U + KE,3));
        }
    });
    UIElements.push_back(std::unique_ptr<Label>(mechEnergyLabel));

    Label* kineticEnergyLabel = new Label({20.f,920.f},font);
    kineticEnergyLabel->setLiveUpdate([this,kineticEnergyLabel](){
        if(this->selectedObject!=nullptr){
            kineticEnergyLabel->setText("KE=" + formatFloatToSigFigs(
                0.5f * this->selectedObject->getMass() * this->selectedObject->getVel().lengthSquared(),3
            ));
        }
    });
    UIElements.push_back(std::unique_ptr<Label>(kineticEnergyLabel));

    Label* potentialEnergyLabel = new Label({20.f,940.f},font);
    potentialEnergyLabel->setLiveUpdate([this,potentialEnergyLabel](){
        if(this->selectedObject!=nullptr){
            potentialEnergyLabel->setText("U=" + formatFloatToSigFigs(
                this->selectedObject->getMass() * this->gravity *
                (this->simBounds.position.y + this->simBounds.size.y - this->selectedObject->getPos().y),3
            ));
        }
    });
    UIElements.push_back(std::unique_ptr<Label>(potentialEnergyLabel));
    
}