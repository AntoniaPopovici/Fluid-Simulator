#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "Shader.h"
#include "ParticleBuffers.h"
#include "ParticleGenerator.h"
#include "ComputeShader.h"


class ParticleRenderer {
public:
    struct ParticleData {
        std::vector<glm::vec2> positions;
        std::vector<glm::vec2> velocities;
        std::vector<glm::vec2> predictedPositions;
        std::vector<glm::vec2> densities;
        std::vector<glm::uvec3> spatialIndices;
        std::vector<GLuint> spatialOffsets;
    };

    ParticleRenderer(size_t particleCount, Shader* shader, ComputeShader* computeShader, const ParticleGenerator::ParticleSpawnData& spawnData);
    ~ParticleRenderer();

    void UpdateParticles();
    void useComputeShader();
    bool validateParticleData(GLuint particleCount, GLuint numThreads);
    void updateBuffer(GLuint buffer, const std::vector<glm::vec2>& data, const std::string& bufferName);
    void RetrieveAndDebugData();
    void DrawParticles();

    ParticleData& GetParticleData();
    ParticleBuffers* GetParticleBuffers() const;
    void DebugParticleData();
    void DebugAdditionalBufferData(const std::vector<glm::uint>& debugValues);

private:
    Shader* shader;
    ComputeShader* computeShader; 
    ParticleBuffers* particleBuffers;
    ParticleData particleData;
    GLuint VAO, VBO;
    GLuint positionVBO, velocityVBO;

    static const int NumThreads = 64;

    void InitRenderBuffers();
    void InitParticleData(size_t particleCount, const ParticleGenerator::ParticleSpawnData& spawnData);
    void CheckGLError(const std::string& operation);
};

#endif // PARTICLE_RENDERER_H
