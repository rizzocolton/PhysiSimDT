#include <SFML/Graphics.hpp>
#include <iostream>
#include "Physics/Constants.h"
#include "Physics/SpatialMap.h"
#include "Physics/Objects/Circle.h"
#include "UI/Button.h"
#include "UI/Slider.h"

int main(){

    //** SETUP */


    //WINDOWS
    
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(SCREEN_WIDTH),static_cast<unsigned int>(SCREEN_HEIGHT)}), "PhysiSim",sf::Style::Close);
    window.setPosition({500,0});
    sf::RenderWindow controls(sf::VideoMode({500,600}),"Controls",sf::Style::Titlebar);
    controls.setPosition({0,0});
    sf::RenderWindow object(sf::VideoMode({500,400}),"Object",sf::Style::Titlebar);
    object.setPosition({0,600});

    //close up of a specific object
    sf::View objectView({SCREEN_WIDTH/2.f,SCREEN_HEIGHT/2.f},{250.f,250.f});
    objectView.setViewport(sf::FloatRect({0.6f, 0.f}, {0.4f, 0.5f}));
    //pointer to that object
    Circle* selectedObject=nullptr;
    
    //SIMULATION SCREEN

    loadFonts();
    sf::Text fpsCounter(ICELAND);
    fpsCounter.setPosition({1.f,0.f});

    sf::Text population(ICELAND);
    population.setPosition({1.f,20.f});

    //SIMULATION VARIABLES
    
    
    bool simulating=false;
    //Objects container
    std::vector<Circle> objects;
    int fps=0;
    sf::Clock oneSecond;
    sf::Clock timeSinceLastFrame;
    
    sf::Vector2i mousePos;

    //Collision specific
    SpatialMap sm=SpatialMap(50);
    

    //CONTROLS SCREEN

     

    Button start_Stop{sf::Vector2f{10.f,50.f}, sf::Vector2f{200.f,200.f}};
    start_Stop.setText("Start");
    start_Stop.setOnClick([&simulating,&start_Stop](){
        if(simulating){
            simulating=false;
            start_Stop.setText("Start");
        }else{
            simulating=true;
            start_Stop.setText("Stop");
        }
    });
    
    Slider gravitySlider(
        sf::Vector2f{10.f,400.f}, //position
        sf::Vector2f{200.f,50.f}, //size
        0.f, //min value
        500.f, //max value
        GRAVITY //current value
    );

    //OBJECT SCREEN

    Slider massSlider(
        sf::Vector2f{10.f,10.f}, //position
        sf::Vector2f{200.f,50.f}, //size
        1.f, //min value
        100.f, //max value
        10.f //current value
    );

    Slider radiusSlider(
        sf::Vector2f{10.f,100.f}, //position
        sf::Vector2f{200.f,50.f}, //size
        5.f, //min value
        50.f, //max value
        10.f //current value
    );

    Slider positionSlider(
        sf::Vector2f{10.f,300.f}, //position
        sf::Vector2f{200.f,50.f}, //size
        0.f, //min value
        1400.f, //max value
        700.f //current value
    );

    Slider velocitySlider(
        sf::Vector2f{10.f,200.f}, //position
        sf::Vector2f{200.f,50.f}, //size
        0.f, //min value
        500.f, //max value
        0.f //current value
    );

    Slider angleSlider(
        sf::Vector2f{10.f,400.f}, //position
        sf::Vector2f{200.f,50.f}, //size
        0.f, //min value
        360.f, //max value
        0.f //current value
    );


    //* MAIN LOOP */
    while(window.isOpen()){
        //FIND MOUSE LOCATION; USEFUL FOR MANY INTERACTIONS
        mousePos=sf::Mouse::getPosition(controls);
        //checks all window events that were triggered since last loop
        while(const auto event = window.pollEvent()){   
            //if close is requested, then close the window
            if (event->is<sf::Event::Closed>()){
                window.close();
                controls.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape){
                    window.close();
                    controls.close();
                }
                if(keyPressed->scancode==sf::Keyboard::Scan::Space){
                    start_Stop.runOnClick();
                }
                if(keyPressed->scancode==sf::Keyboard::Scan::Tab){
                    controls.setVisible(true);
                }
            }
            
            if(const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                //if the left mouse is pressed, put an object at that positon
                if(mouseButtonPressed->button==sf::Mouse::Button::Left){
                    sf::Vector2f mousePos(mouseButtonPressed->position.x,mouseButtonPressed->position.y);
                    for(int i=0;i<100;i++){
                        objects.emplace_back(
                            mousePos,
                            5,
                            rand()%10+1,
                            sf::Color(rand()%255,rand()%255,rand()%255)
                        );
                    }
                }
                //if the right mouse is pressed, get the nearest object at that position
                if(mouseButtonPressed->button==sf::Mouse::Button::Right){
                    //mouse's current position
                    sf::Vector2f mousePos(mouseButtonPressed->position.x,mouseButtonPressed->position.y);
                    //set of all objects at that cell
                    std::unordered_set objs=sm.getMap()[sm.getKey(mousePos)];
                    //vector pointing to every object in that cell
                    std::vector<Circle*> objVec;
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
                            float distSqrd=(mousePos-objVec.at(i)->getPos()).lengthSquared();
                            if(distSqrd<minDistSqrd){
                                minDistSqrd=distSqrd;
                                minIndex=i;
                            }
                        }
                        if(selectedObject!=nullptr){
                            selectedObject->unhighlight();
                        }
                        selectedObject=objVec.at(minIndex);
                        objVec.at(minIndex)->highlight();
                    }
                }
            }
            
        }
        
        
        while(const std::optional event = controls.pollEvent()){

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape){
                    window.close();
                    controls.close();
                }
                if(keyPressed->scancode==sf::Keyboard::Scan::Space){
                    start_Stop.runOnClick();
                }
                if(keyPressed->scancode==sf::Keyboard::Scan::Tab){
                    controls.setVisible(false);
                }
            }
        }

        

        //* PRIMARY WINDOW RENDERING */

        //SIMULATION

        window.clear();
        sm.draw(window);
        sm.clear();
        //time elapsed since last frame
        float dt=timeSinceLastFrame.restart().asSeconds();
        if(!simulating){
            dt=0.f; //Nothing will simulate if no time has passed.
        }

        //updates and draws every object created
        for(auto& obj:objects){
            sf::Vector2f weight{0.0f,obj.getMass()*GRAVITY};
            obj.push(weight,dt);
            obj.update(dt);
            sm.enterCell(&obj);
            obj.draw(window);
        }

        //checks for collisions with every object in every cell and adjacent cells
        for(auto& [cell,objs]: sm.getMap()){
            //makes a vector of all objs in set
            std::vector<Circle*> objVec(objs.begin(),objs.end());

            for(int i=0;i<objVec.size();i++){
                GridKey currentKey=sm.getKey(*objVec[i]);
                int cellSize=sm.getCellSize();
                sf::Vector2f objPos=objVec[i]->getPos();
    
                // Check 3x3 grid 
                for(int dx=-1;dx<=1;dx++){
                    for(int dy=-1;dy<=1;dy++){
                        sf::Vector2f adjPos(
                            objPos.x+dx*cellSize, 
                            objPos.y+dy*cellSize
                        );
                        GridKey adjKey(adjPos,cellSize);
            
                        auto& map=sm.getMap();
                        if(map.count(adjKey)!=0){
                            for(Circle* otherObj : map[adjKey]){
                                objVec[i]->collide(*otherObj);
                            }
                        }
                    }
                }
            }
        }//end simulation frame
        fps++;

        //display fps
        if(oneSecond.getElapsedTime().asSeconds()>=1){
            fpsCounter.setString("FPS: "+std::to_string(fps));
            oneSecond.restart();
            fps=0;
        }
        window.draw(fpsCounter);

        //display population
        population.setString("Population: "+std::to_string(objects.size()));
        window.draw(population);

        window.display();

        //* CONTROL WINDOW RENDERING */
        
        controls.clear();

        bool mouseClicked=sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        start_Stop.update(mousePos,mouseClicked);
        start_Stop.draw(controls);

        
        gravitySlider.update(mousePos,mouseClicked);
        gravitySlider.draw(controls);

        GRAVITY=static_cast<float>(gravitySlider.getValue());


        controls.display();

        //* OBJECT WINDOW RENDERING */

        mousePos=sf::Mouse::getPosition(object);
        
        object.clear();

        
        
        if(massSlider.update(mousePos,mouseClicked) && selectedObject!=nullptr){
            selectedObject->setMass(massSlider.getValue());
        }
        massSlider.draw(object);

        
        
        if(radiusSlider.update(mousePos,mouseClicked) && selectedObject!=nullptr){
            selectedObject->setRadius(radiusSlider.getValue());
        }
        radiusSlider.draw(object);

        object.display();
    }
}