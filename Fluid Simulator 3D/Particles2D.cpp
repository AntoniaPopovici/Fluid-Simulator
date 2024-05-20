#include "Particles2D.h"
#include <iostream>

Particles2D::Particles2D(Shader* &shader) : shader(shader) {
    shader->use();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)(sizeof(float) * 2)); // Color
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


Particles2D::~Particles2D()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}


void Particles2D::addPoint(float x, float y, float r, float g, float b, float dx, float dy) {
    points.push_back({ x, y, r, g, b, dx, dy });
    updateVertexBuffer();
}

void Particles2D::draw() {
    shader->use();
    float pointSize = 10.0f;
    
    //int width = 1024;
    //int height = 768;
    //glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
    
    //shader->setMat4("projection", projection);
    //shader->setFloat("pointSize", pointSize);

    if (points.empty()) {
        std::cout << "No points to draw." << std::endl;
        return;
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

    glPointSize(pointSize);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, points.size());
    glBindVertexArray(0);
}

void Particles2D::update(float deltaTime) {
    for (auto& point : points) {
        point.x += point.dx * deltaTime;
        point.y += point.dy * deltaTime;
        std::cout << point.x << " " << point.y << "   " 
                  << point.r << " " << point.g << " " << point.b << '\n';
    }
    std::cout << '\n';
    updateVertexBuffer();
}

void Particles2D::deletePoints()
{
    points.clear();
}


void Particles2D::updateVertexBuffer() 
{
    shader->use();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * points.size(), NULL, GL_DYNAMIC_DRAW); // Allocate space
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Point) * points.size(), points.data()); // Copy data
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
