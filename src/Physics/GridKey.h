#ifndef GRIDKEY_H
#define GRIDKEY_H

#include <SFML/Graphics.hpp>


struct GridKey{
    int x;
    int y;

    GridKey(sf::Vector2f p,int cellSize){
        x=static_cast<int>(p.x)/cellSize;
        y=static_cast<int>(p.y)/cellSize;
    }

    //default constructor so the compiler will stop complaining when initializing circles
    GridKey(){
        x=0;
        y=0;
    }

    //found this online, needed to make sure that equalities work between GridKeys
    bool operator==(const GridKey& other) const{
        return x==other.x&&y==other.y;
    }
};

struct GridKeyHash{
    //also found this online, a simple hash depending on x and y
    std::size_t operator()(const GridKey& gk) const{
        //hashes each component
        std::size_t h1=std::hash<int>{}(gk.x);
        std::size_t h2=std::hash<int>{}(gk.y);
        
        //then combines components
        return h1^(h2<<1);
    }
};

#endif