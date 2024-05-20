#include "Particle2DGenerator.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Particle2DGenerator::Particle2DGenerator(
    float x,
    float y,
    float spread,
    int maxParticles,
    float screenWidth,
    float screenHeight,
    Shader*& shader)
    :
    sourceX(x),
    sourceY(y),
    spread(spread),
    maxParticles(maxParticles),
    screenWidth(screenWidth),
    screenHeight(screenHeight),
    particleSystem(shader)
{
    std::srand(std::time(nullptr)); 
}


Particle2DGenerator::~Particle2DGenerator() {

}

void Particle2DGenerator::generate(float deltaTime) {
    while (particleSystem.points.size() < maxParticles) {
        float dx = generateRandom(-spread, spread);
        float dy = generateRandom(-spread, spread);
        float r = generateRandom(0, 1);
        float g = generateRandom(0, 1);
        float b = generateRandom(0, 1);
        particleSystem.addPoint(sourceX, sourceY, r, g, b, dx, dy);
    }
}

void Particle2DGenerator::update(float deltaTime) {
    for (auto& particle : particleSystem.points) {
        if (particle.x <= 0 || particle.x >= screenWidth) {
            particle.dx *= -1;
        }
        if (particle.y <= 0 || particle.y >= screenHeight) {
            particle.dy *= -1;
        }
        applyPhysics(particle, deltaTime);
    }
    particleSystem.update(deltaTime);
}

void Particle2DGenerator::draw() {
    particleSystem.draw();
}

float Particle2DGenerator::generateRandom(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void Particle2DGenerator::applyPhysics(Particles2D::Point& particle, float deltaTime) {
    particle.dy -= 9.8f * deltaTime; // Gravity effect
}
