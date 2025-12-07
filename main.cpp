#include <SFML/Graphics.hpp>
#include <iostream>


//OBJECT SETUP

//An object which has a position, can be moved by a velocity, and can be pushed by a force
float screenWidth=1600.f;
float screenHeight=1000.f;

class PhysicsObject{
protected:
    sf::Vector2f pos;
    sf::Vector2f vel;
    float mass;
    bool boundaryCollision;

public:
    PhysicsObject(bool constrained=true)
        :boundaryCollision(constrained){}

    virtual void checkBounds()=0;
    
    void update(float dt){
        pos+=vel*dt;
        if(boundaryCollision){
            checkBounds();
        }
    }

    void push(sf::Vector2f force,float dt){
        vel+=(force/mass)*dt;
    }

    void draw(sf::RenderWindow& window);

    float getMass(){
        return mass;
    }    
    sf::Vector2f getPos(){
        return pos;
    }
    sf::Vector2f getVel(){
        return vel;
    }
};


class Circle : public PhysicsObject{
private:
    sf::CircleShape shape;

public:
    //creates a circle at (x,y) with a particular radius and color
    Circle(sf::Vector2f place, float radius, float m, sf::Color color){
        shape.setRadius(radius);
        shape.setPosition(place);
        pos=place;
        mass=m;
    }

    //renders the circle onto whatever window is passed in
    void draw(sf::RenderWindow& window){
        shape.setPosition(pos);
        window.draw(shape);
    }

    //overrides virtual check boundaries for circular geometry
    //origin for circle is top left, hence the odd looking inequalities
    void checkBounds() override{
        if(pos.x<0||pos.x+2*shape.getRadius()>screenWidth){
            vel.x*=-1;
        }
        if(pos.y<0||pos.y+2*shape.getRadius()>screenHeight){
            vel.y*=-1;
        }
    }
};


int main()
{

    //** SETUP */


    //DISPLAY SETUP

    //creating display & control windows 
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(screenWidth),static_cast<unsigned int>(screenHeight)}), "PhysiSim",sf::Style::Close);
    window.setPosition({300,100});
    sf::RenderWindow controls(sf::VideoMode({300,800}),"Controls",sf::Style::Titlebar);
    controls.setPosition({0,100});

    //loading text and ui elements
    sf::Font font("../assets/Iceland-Regular.ttf");

    //vector to hold all controlsText
    std::vector<sf::Text> controlsText;

    //creating title
    sf::Text controlTitle(font);
    controlTitle.setString("Controls");
    controlTitle.setCharacterSize(24);
    controlTitle.setFillColor(sf::Color::White);
    controlsText.push_back(controlTitle);

    for(sf::Text text : controlsText){
        controls.draw(text);
    }
    controls.display();


    
    //Objects container
    std::vector<Circle> objects;

    //* MAIN LOOP */
    sf::Clock clock;
    while(window.isOpen()){
        //checks all window events that were triggered since last loop
        while(const auto event = window.pollEvent()){   
            //if close is requested, then close the window
            if (event->is<sf::Event::Closed>()){
                window.close();
                controls.close();
            }
            //if escape is pressed, close all windows
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scan::Escape){
                    window.close();
                    controls.close();
                }
            }
            //if the mouse is pressed, put an object at that positon
            if(const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                if(mouseButtonPressed->button==sf::Mouse::Button::Left){
                    sf::Vector2f mousePos(mouseButtonPressed->position.x,mouseButtonPressed->position.y);
                    objects.emplace_back(mousePos,20,1,sf::Color::Red);
                }
            }
            
        }

        while(const std::optional event = controls.pollEvent()){   
            
        }

        //* PRIMARY WINDOW RENDERING */

        //clear frame
        window.clear();

        //time elapsed since last frame
        float dt=clock.restart().asSeconds();
        std::cout<<dt<<std::endl;

        //updates and draws every object created
        for(auto& obj:objects){
            sf::Vector2f weight{0.0f,obj.getMass()*9.8f};
            obj.push(weight,dt);
            obj.update(dt);
            obj.draw(window);
        }

        //end frame
        window.display();

        //* CONTROL WINDOW RENDERING */

        
    }
}