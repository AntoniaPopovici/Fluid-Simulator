#pragma once
#include <vector>
#include <GL/glew.h> // Ensure you have GLEW or another OpenGL loader initialized
#include <cmath>
#include "Shader.h"

constexpr float PI = 3.14159265358979323846f;

class Particle2DGenerator;

class Particles2D {
public:
    struct Point {
        float x, y; // Coordinates of the point
        float r, g, b; // Color of the point
        float dx, dy; // Direction vector components
    };

    Particles2D(Shader* &shader);
    ~Particles2D();

    void addPoint(float x, float y, float r, float g, float b, float dx, float dy);
    void deletePoints();
    void draw();
    void debugDrawCall();
    void update(float deltaTime);

    friend class Particle2DGenerator; 
private:
    std::vector<Point> points;
    GLuint VBO, VAO;
    Shader* shader;
    void updateVertexBuffer();
};
