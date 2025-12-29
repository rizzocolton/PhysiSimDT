#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <functional>

class UI{
    protected:
        sf::Vector2f pos;

        //optional subclass functions
        std::function<void()> onChange;
        std::function<void()> liveUpdate;

    public:
        UI(sf::Vector2f p): pos(p) {};

        virtual void draw(sf::RenderWindow& window)=0;

        virtual void handleEvent(const sf::Event& event)=0;

        //the following 4 functions are for adding functionality and responsiveness to any UI element

        void setOnChange(std::function<void()> func){
            onChange=func;
        }

        void runOnChange(){
            if(onChange){
                onChange();
            }
        }

        void setLiveUpdate(std::function<void()> func){
            liveUpdate=func;
        }

        void runLiveUpdate(){
            if(liveUpdate){
                liveUpdate();
            }
        }

        //for copying into saves
        virtual std::unique_ptr<UI> clone() const=0;

        virtual ~UI()=default;
};

#endif