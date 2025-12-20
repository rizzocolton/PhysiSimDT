#include "Collisions.h"
#include "../../UI/Button.h"
#include "../../UI/Slider.h"

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


    Slider* gravitySlider = new Slider({5.f,65.f},{200.f,50.f},0.f,100.f,gravity/10.f);
    gravitySlider->setOnChange([this,gravitySlider](){
        gravity=10*gravitySlider->getValue();
    });
    UIElements.push_back(std::unique_ptr<Slider>(gravitySlider));

    Slider* colRestitutionSlider = new Slider({5.f,125.f},{200.f,50.f},0.f,1.f,colRestitution);
    colRestitutionSlider->setOnChange([this,colRestitutionSlider](){
        colRestitution=colRestitutionSlider->getValue();
    });
    UIElements.push_back(std::unique_ptr<Slider>(colRestitutionSlider));

    Slider* boundsRestitutionSlider = new Slider({5.f,185.f},{200.f,50.f},0.f,1.f,boundsRestitution);
    boundsRestitutionSlider->setOnChange([this,boundsRestitutionSlider](){
        boundsRestitution=boundsRestitutionSlider->getValue();
    });
    UIElements.push_back(std::unique_ptr<Slider>(boundsRestitutionSlider));

}

void Collisions::drawUI(sf::RenderWindow& window){
    //Placeholder for future UI elements
    for(auto& element : UIElements){
        element->draw(window);
    }
}

void Collisions::handleEvent(const sf::Event& event){
    //if space pressed, toggle simulation state
    if(event.getIf<sf::Event::KeyPressed>()){
        if(event.getIf<sf::Event::KeyPressed>()->scancode==sf::Keyboard::Scancode::Space){
            simulating = !simulating;
        }
    }
    
    //if mouse pressed on sim screen, add a new circle at mouse position
    if(event.getIf<sf::Event::MouseButtonPressed>()){
        if(event.getIf<sf::Event::MouseButtonPressed>()->button==sf::Mouse::Button::Left){
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

