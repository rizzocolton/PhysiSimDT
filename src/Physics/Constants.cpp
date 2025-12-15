#include "Constants.h"


float GRAVITY=98.f;
float COEFFICENT_OF_RESTITUTION=0.9f;
sf::Font ICELAND;
bool loadFonts(){
    return ICELAND.openFromFile("../assets/Iceland-Regular.ttf");
}
