#include "Collisions.h"
#include "../../UI/Button.h"
#include "../../UI/Slider.h"
#include "../../UI/Spinner.h"
#include <iostream>



Collisions::Collisions(float gravity, float colRestitution, float boundsRestitution, int cellSize, sf::FloatRect bounds)
: gravity(gravity), colRestitution(colRestitution), boundsRestitution(boundsRestitution), sm(cellSize,bounds), simBounds(bounds){}

void Collisions::update(float dt){
    //If simulation is not running, skip physics update
    if(!simulating){
        return;
    }

    //Clear spatial map for new frame
    sm.clear();

    //Update position of all objects
    for(auto& obj : objects){
        sf::Vector2f weight{0.0f,obj->getMass()*gravity};
        obj->push(weight,dt);
        obj->update(dt);
        obj->checkBounds(simBounds,boundsRestitution);
        sm.enterCell(obj.get());
    }

    //Check for collisions between objects in cells and adjacent cells
    for(auto& [cell,objs]: sm.getMap()){
        //makes a vector of all objs in set
        std::vector<PhysicsObject*> objVec(objs.begin(),objs.end());
        for(int i=0;i<objVec.size();i++){
            for(int j=i+1;j<objVec.size();j++){
                objVec[i]->collide(*objVec[j],colRestitution);
            }

        //check adjacent cells
        
            //right cell
            GridKey rightKey(cell.getX()+1,cell.getY());
            if(sm.getMap().count(rightKey)!=0){
                for(auto& otherObj : sm.getMap().at(rightKey)){
                    objVec[i]->collide(*otherObj,colRestitution);
                }
            }

            //bottom cell
            GridKey bottomKey(cell.getX(),cell.getY()+1);
            if(sm.getMap().count(bottomKey)!=0){
                for(auto& otherObj : sm.getMap().at(bottomKey)){
                    objVec[i]->collide(*otherObj,colRestitution);
                }
            }

            //bottom-right cell
            GridKey bottomRightKey(cell.getX()+1,cell.getY()+1);
            if(sm.getMap().count(bottomRightKey)!=0){
                for(auto& otherObj : sm.getMap().at(bottomRightKey)){
                    objVec[i]->collide(*otherObj,colRestitution);
                }
            }

            //bottom-left cell
            GridKey bottomLeftKey(cell.getX()-1,cell.getY()+1);
            if(sm.getMap().count(bottomLeftKey)!=0){
                for(auto& otherObj : sm.getMap().at(bottomLeftKey)){
                    objVec[i]->collide(*otherObj,colRestitution);
                }
            }
        }
    }
}

void Collisions::draw(sf::RenderWindow& window){
    //Optionally draw spatial map grid
    //sm.draw(window);

    //Draw all objects
    for(auto& obj : objects){
        obj->draw(window);
    }
}

void Collisions::initUI(sf::Font& font){
    //CORE SIM CONTROL BUTTONS

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


    //SIMULATION PARAMETER CONTROLS


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


    //DETAIL PANEL

    Spinner* posXSpinner = new Spinner({20.f,700.f},{100.f,40.f}, font,-1000.f,1000.f,0.f);
    posXSpinner->setOnChange([this,posXSpinner](){
        //if selected object exists, update its x position
        if(this->selectedObject!=nullptr){
            this->selectedObject->setPos({posXSpinner->getValue(),selectedObject->getPos().y});
        }
        posXSpinner->setText("Pos X: " + formatFloatToDecimalPlaces(posXSpinner->getValue(),2));
    });
    posXSpinner->setLiveUpdate([this,posXSpinner](){
        //if selected object exists, update spinner text to match its x position
        if(this->selectedObject!=nullptr){
             posXSpinner->setValue(this->selectedObject->getPos().x);
        }
        posXSpinner->setText("Pos X: " + formatFloatToDecimalPlaces(posXSpinner->getValue(),2));
    });
    posXSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(posXSpinner));

    Spinner* posYSpinner = new Spinner({220.f,700.f},{100.f,40.f}, font,-1000.f,1000.f,0.f);
    posYSpinner->setOnChange([this,posYSpinner](){
        //if selected object exists, update its y position
        if(this->selectedObject!=nullptr){
            this->selectedObject->setPos({selectedObject->getPos().x,posYSpinner->getValue()});
        }
        posYSpinner->setText("Pos Y: " + formatFloatToDecimalPlaces(posYSpinner->getValue(),2));
    });
    posYSpinner->setLiveUpdate([this,posYSpinner](){
        //if selected object exists, update spinner text to match its y position
        if(this->selectedObject!=nullptr){
             posYSpinner->setValue(this->selectedObject->getPos().y);
        }
        posYSpinner->setText("Pos Y: " + formatFloatToDecimalPlaces(posYSpinner->getValue(),2));
    });
    posYSpinner->runOnChange();
    UIElements.push_back(std::unique_ptr<Spinner>(posYSpinner));
}

void Collisions::drawUI(sf::RenderWindow& window){
    for(auto& element : UIElements){
        element->draw(window);
    }
}

void Collisions::handleEvent(const sf::Event& event){
    //useful for a lot of event handling
    sf::Vector2i mousePos=sf::Mouse::getPosition();

    //if space pressed, toggle simulation state
    if(event.getIf<sf::Event::KeyPressed>()){
        if(event.getIf<sf::Event::KeyPressed>()->scancode==sf::Keyboard::Scancode::Space){
            simulating = !simulating;
        }
    }
    
    //if mouse pressed on sim screen, add a new circle at mouse position
    if(event.getIf<sf::Event::MouseButtonPressed>()){
        if(event.getIf<sf::Event::MouseButtonPressed>()->button==sf::Mouse::Button::Left){
            //if mouse is within simulation bounds, place a new circle
            if(mousePos.x>simBounds.position.x && mousePos.x<simBounds.position.x+simBounds.size.x &&
               mousePos.y>simBounds.position.y && mousePos.y<simBounds.position.y+simBounds.size.y){
                sf::Vector2f position{static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)};
                float radius=10.f;
                float mass=5;
                sf::Color color(
                    (rand()%256),
                    (rand()%256),
                    (rand()%256)
                );
                objects.push_back(std::make_unique<Circle>(position, radius, mass, color));
            }
        }

        //if the right mouse is pressed, get the nearest object at that position
        if(event.getIf<sf::Event::MouseButtonPressed>()->button==sf::Mouse::Button::Right){
            //mouse's current position
            sf::Vector2f mousePosf(mousePos.x,mousePos.y);
            //set of all objects at that cell
            std::unordered_set objs=sm.getMap()[sm.getKey(mousePosf)];
            //vector pointing to every object in that cell
            std::vector<PhysicsObject*> objVec;
            //filling that vector using extraction (C++17 exclusive thing)
            objVec.reserve(objs.size());
            while(!objs.empty()){
                objVec.push_back(std::move(objs.extract(objs.begin()).value()));
            }
            //finds the index of the object closest to the mouse
            if(!objVec.empty()){
                float minDistSqrd=static_cast<float>(sm.getCellSize())*static_cast<float>(sm.getCellSize());
                int minIndex=0;
                for(int i=0;i<objVec.size();i++){
                    float distSqrd=(mousePosf-objVec.at(i)->getPos()).lengthSquared();
                    if(distSqrd<minDistSqrd){
                        minDistSqrd=distSqrd;
                        minIndex=i;
                    }
                }
                selectedObject=objVec.at(minIndex);
                std::cout<<"Selected object at position: "<<selectedObject->getPos().x<<","<<selectedObject->getPos().y<<"\n";
            }
        }
    }

    //if mouse is held down and moved, add circles along the drag path
    if(event.getIf<sf::Event::MouseMoved>()){
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            sf::Vector2i mousePos=sf::Mouse::getPosition();
            //if mouse is within simulation bounds, place a new circle
            if(mousePos.x>simBounds.position.x && mousePos.x<simBounds.position.x+simBounds.size.x &&
               mousePos.y>simBounds.position.y && mousePos.y<simBounds.position.y+simBounds.size.y){
                sf::Vector2f position{static_cast<float>(mousePos.x),static_cast<float>(mousePos.y)};
                float radius=10.f;
                float mass=5;
                sf::Color color(
                    (rand()%256),
                    (rand()%256),
                    (rand()%256)
                );
                objects.push_back(std::make_unique<Circle>(position, radius, mass, color));
            }
        }
    }

    //update all UI elements
    for(auto& element : UIElements){
        element->handleEvent(event);
    }
}

int Collisions::getPopulation(){
    return static_cast<int>(objects.size());
}

