#pragma once
#include "Particles2D.h"
#include <vector>

class Particle2DGenerator {
public:
    Particle2DGenerator(float x, float y, float spread, int maxParticles, float screenWidth, float screenHeight, Shader*& shader);
    ~Particle2DGenerator();

    void generate(float deltaTime);
    void update(float deltaTime);
    void draw();

private:
    float screenWidth, screenHeight; // Screen width and height for boundary checks
    float sourceX, sourceY; // The source position of the particles
    float spread; // Spread of initial particle direction
    int maxParticles; // Maximum number of particles
    std::vector<Particles2D::Point> particles;
    Particles2D particleSystem;

    Shader* shader;

    float generateRandom(float min, float max); // Utility to generate random float
    void applyPhysics(Particles2D::Point& particle, float deltaTime); // Apply physics to particles
};
