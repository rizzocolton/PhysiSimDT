#include "ObjectRenderer.h"
#include <iostream>

//declaring static member variable so linker doesn't whine
std::vector<sf::Vertex> ObjectRenderer::vertices;
sf::Texture ObjectRenderer::circleTexture;

//A lot of this is AI generated as sfml textures are certainly not my forte
void ObjectRenderer::initTexture() {
    unsigned int size = 512;
    sf::RenderTexture rt;
    auto rtResult = rt.resize({size, size});
    if (!rtResult) {
        std::cerr << "ERROR: Failed to create RenderTexture for ObjectRenderer" << std::endl;
        return;
    }
    rt.clear(sf::Color::Transparent);

    sf::CircleShape shape(size / 2.f);
    shape.setFillColor(sf::Color::White); // White allows vertex colors to work
    shape.setPointCount(100); // High point count for the "master" texture
    
    rt.draw(shape);
    rt.display();

    circleTexture = sf::Texture(rt.getTexture());
    circleTexture.setSmooth(true); // This is key for the "perfect" look
}

void ObjectRenderer::clear(){
    vertices.clear();
};

void ObjectRenderer::addCircle(float x, float y, float radius, sf::Color color, float scaleFactor, float xOffset, float yOffset){
    float xPixels=(x+xOffset)*scaleFactor;
    float yPixels=(y+yOffset)*scaleFactor;
    float rPixels=radius*scaleFactor;

    // Define the 4 points of the diamond
   // Define the 4 corners of a square centered at xPixels, yPixels
    sf::Vector2f tl(xPixels - rPixels, yPixels - rPixels); // Top-Left
    sf::Vector2f tr(xPixels + rPixels, yPixels - rPixels); // Top-Right
    sf::Vector2f bl(xPixels - rPixels, yPixels + rPixels); // Bottom-Left
    sf::Vector2f br(xPixels + rPixels, yPixels + rPixels); // Bottom-Right

    float ts = 512.f; 
    sf::Vector2f uvTL(0.f, 0.f);
    sf::Vector2f uvTR(ts, 0.f);
    sf::Vector2f uvBL(0.f, ts);
    sf::Vector2f uvBR(ts, ts);

    // Triangle 1: Top-Left, Top-Right, Bottom-Left
    vertices.push_back(sf::Vertex{tl, color, uvTL});
    vertices.push_back(sf::Vertex{tr, color, uvTR});
    vertices.push_back(sf::Vertex{bl, color, uvBL});

    // Triangle 2: Bottom-Left, Top-Right, Bottom-Right
    vertices.push_back(sf::Vertex{bl, color, uvBL});
    vertices.push_back(sf::Vertex{tr, color, uvTR});
    vertices.push_back(sf::Vertex{br, color, uvBR});
};

void ObjectRenderer::draw(sf::RenderWindow& window){
    // Validate that we have vertices and a valid texture before drawing
    if (vertices.empty()) {
        return;  // Nothing to draw
    }
    
    // Draw with texture - SFML will handle null texture gracefully, but we add safety check
    window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Triangles, &circleTexture);
};