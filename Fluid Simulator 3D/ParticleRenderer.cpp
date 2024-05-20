#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer(size_t particleCount, Shader* shader, ComputeShader* computeShader, const ParticleGenerator::ParticleSpawnData& spawnData)
    : shader(shader), computeShader(computeShader) { 
    particleBuffers = new ParticleBuffers(particleCount);
    InitParticleData(particleCount, spawnData); 
    InitRenderBuffers(); 
}

ParticleRenderer::~ParticleRenderer() {
    delete particleBuffers;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ParticleRenderer::InitRenderBuffers() {
    // Generate and bind the Vertex Array Object
    glGenVertexArrays(1, &VAO);
    CheckGLError("ParticleRenderer::InitRenderBuffers - GenVertexArrays");

    glBindVertexArray(VAO);
    CheckGLError("ParticleRenderer::InitRenderBuffers - BindVertexArray");

    // Generate and bind the position buffer
    glGenBuffers(1, &positionVBO);
    CheckGLError("ParticleRenderer::InitRenderBuffers - GenPositionBuffers");

    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    CheckGLError("ParticleRenderer::InitRenderBuffers - BindPositionBuffer");

    if (!particleData.positions.empty()) {
        glBufferData(GL_ARRAY_BUFFER, particleData.positions.size() * sizeof(glm::vec2), particleData.positions.data(), GL_DYNAMIC_DRAW);
        CheckGLError("ParticleRenderer::InitRenderBuffers - PositionBufferData");
    }
    else {
        std::cerr << "Error: particleData.positions is empty during buffer allocation." << std::endl;
    }

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    CheckGLError("ParticleRenderer::InitRenderBuffers - PositionVertexAttribPointer");

    glEnableVertexAttribArray(0);
    CheckGLError("ParticleRenderer::InitRenderBuffers - EnablePositionVertexAttribArray");

    // Generate and bind the velocity buffer
    glGenBuffers(1, &velocityVBO);
    CheckGLError("ParticleRenderer::InitRenderBuffers - GenVelocityBuffers");

    glBindBuffer(GL_ARRAY_BUFFER, velocityVBO);
    CheckGLError("ParticleRenderer::InitRenderBuffers - BindVelocityBuffer");

    if (!particleData.velocities.empty()) {
        glBufferData(GL_ARRAY_BUFFER, particleData.velocities.size() * sizeof(glm::vec2), particleData.velocities.data(), GL_DYNAMIC_DRAW);
        CheckGLError("ParticleRenderer::InitRenderBuffers - VelocityBufferData");
    }
    else {
        std::cerr << "Error: particleData.velocities is empty during buffer allocation." << std::endl;
    }

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    CheckGLError("ParticleRenderer::InitRenderBuffers - VelocityVertexAttribPointer");

    glEnableVertexAttribArray(1);
    CheckGLError("ParticleRenderer::InitRenderBuffers - EnableVelocityVertexAttribArray");

    // Unbind buffers and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CheckGLError("ParticleRenderer::InitRenderBuffers - UnbindBuffer");

    glBindVertexArray(0);
    CheckGLError("ParticleRenderer::InitRenderBuffers - UnbindVertexArray");
}


void ParticleRenderer::InitParticleData(size_t particleCount, const ParticleGenerator::ParticleSpawnData& spawnData) {
    particleData.positions = spawnData.positions;
    particleData.velocities = spawnData.velocities;
    particleData.predictedPositions = spawnData.positions;

    particleData.densities.resize(particleCount);
    particleData.spatialIndices.resize(particleCount);
    particleData.spatialOffsets.resize(particleCount);
    particleBuffers->UpdateData(particleData.positions, particleData.velocities, particleData.predictedPositions, particleData.densities);

    std::cout << "Initializing particle data. Position count: " << particleData.positions.size() << std::endl;
}

void ParticleRenderer::UpdateParticles() {
    CheckGLError("ParticleRenderer::UpdateParticles - Before BindBuffers");

    useComputeShader();
    CheckGLError("ParticleRenderer::UpdateParticles - Use Compute Shader Program");

    GLuint particleCount = static_cast<GLuint>(particleData.positions.size());
    if (!validateParticleData(particleCount, NumThreads)) return;

    computeShader->DispatchComputeShader(particleCount, NumThreads);
    RetrieveAndDebugData(); // Uncomment to debug FluidSimulator Data

    updateBuffer(positionVBO, particleData.positions, "positions");
    updateBuffer(velocityVBO, particleData.velocities, "velocities");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CheckGLError("ParticleRenderer::UpdateParticles - BindBuffer 0");
}

void ParticleRenderer::useComputeShader() {
    computeShader->use();
    GLuint computeShaderID = computeShader->ID;
    glUseProgram(computeShaderID);

    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if (currentProgram != computeShaderID) {
        std::cerr << "Compute shader program is not currently active." << std::endl;
    }
}

bool ParticleRenderer::validateParticleData(GLuint particleCount, GLuint numThreads) {
    if (particleCount == 0 || numThreads == 0) {
        std::cerr << "Invalid particle count or thread size." << std::endl;
        return false;
    }
    return true;
}

void ParticleRenderer::updateBuffer(GLuint buffer, const std::vector<glm::vec2>& data, const std::string& bufferName) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    CheckGLError("ParticleRenderer::UpdateParticles - Bind " + bufferName + " Buffer");

    if (!data.empty()) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(glm::vec2), data.data());
        CheckGLError("ParticleRenderer::UpdateParticles - " + bufferName + " BufferSubData");
    }
    else {
        std::cerr << "Error: particleData." << bufferName << " is empty during buffer update." << std::endl;
    }
}


void ParticleRenderer::RetrieveAndDebugData() {

    particleBuffers->RetrieveData(particleData.positions, particleData.velocities, particleData.predictedPositions, particleData.densities);
    CheckGLError("ParticleRenderer::RetrieveAndDebugData - RetrieveData");

    //particleBuffers->DebugBufferData();
    CheckGLError("ParticleRenderer::RetrieveAndDebugData - DebugBufferData");

    std::vector<glm::uint> debugValues;
    //particleBuffers->RetrieveDebugData(debugValues);
    CheckGLError("ParticleRenderer::RetrieveAndDebugData - RetrieveDebugData");
    //DebugAdditionalBufferData(debugValues);
}

void ParticleRenderer::DrawParticles() {
    shader->use();
    CheckGLError("ParticleRenderer::DrawParticles - Use Shader");

    glBindVertexArray(VAO);
    CheckGLError("ParticleRenderer::DrawParticles - BindVertexArray");

    glPointSize(2.0f);
    CheckGLError("ParticleRenderer::DrawParticles - PointSize");

    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particleData.positions.size()));
    CheckGLError("ParticleRenderer::DrawParticles - DrawArrays");

    glBindVertexArray(0);
    CheckGLError("ParticleRenderer::DrawParticles - BindVertexArray 0");
}

ParticleRenderer::ParticleData& ParticleRenderer::GetParticleData() {
    return particleData;
}

ParticleBuffers* ParticleRenderer::GetParticleBuffers() const {
    return particleBuffers;
}

void ParticleRenderer::DebugParticleData() {
    std::cout << "Particle Data Debug Info:" << std::endl;
    for (size_t i = 0; i < particleData.positions.size(); ++i) {
        std::cout << "Particle " << i << ":" << std::endl;
        std::cout << "  Position: (" << particleData.positions[i].x << ", " << particleData.positions[i].y << ")" << std::endl;
        std::cout << "  Velocity: (" << particleData.velocities[i].x << ", " << particleData.velocities[i].y << ")" << std::endl;
        std::cout << "  Predicted Position: (" << particleData.predictedPositions[i].x << ", " << particleData.predictedPositions[i].y << ")" << std::endl;
    }
}

void ParticleRenderer::DebugAdditionalBufferData(const std::vector<glm::uint>& debugValues) {
    std::cout << "Additional Buffer Data Debug Info:" << std::endl;
    for (size_t i = 0; i < debugValues.size(); i += 8) {
        glm::uint threadId = debugValues[i];
        float posX, posY, velX, velY, predPosX, predPosY;
        memcpy(&posX, &debugValues[i + 1], sizeof(float));
        memcpy(&posY, &debugValues[i + 2], sizeof(float));
        memcpy(&velX, &debugValues[i + 3], sizeof(float));
        memcpy(&velY, &debugValues[i + 4], sizeof(float));
        memcpy(&predPosX, &debugValues[i + 5], sizeof(float));
        memcpy(&predPosY, &debugValues[i + 6], sizeof(float));
        glm::uint marker = debugValues[i + 7];

        std::cout << "Debug Values for Thread " << threadId << ":" << std::endl;
        std::cout << "  Position X: " << posX << std::endl;
        std::cout << "  Position Y: " << posY << std::endl;
        std::cout << "  Velocity X: " << velX << std::endl;
        std::cout << "  Velocity Y: " << velY << std::endl;
        std::cout << "  Predicted Position X: " << predPosX << std::endl;
        std::cout << "  Predicted Position Y: " << predPosY << std::endl;
        std::cout << "  Debug Marker: " << marker << std::endl;

        if (marker == 1) {
            std::cout << "  Debug Stage: Initial" << std::endl;
        }
        else if (marker == 2) {
            std::cout << "  Debug Stage: After Prediction" << std::endl;
        }
        else if (marker == 3) {
            std::cout << "  Debug Stage: Final" << std::endl;
        }
    }
}

void ParticleRenderer::CheckGLError(const std::string& operation) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error during " << operation << ": " << std::hex << err << std::dec << std::endl;
    }
}