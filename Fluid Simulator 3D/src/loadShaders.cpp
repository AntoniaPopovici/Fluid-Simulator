#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void logShaderInfo(unsigned int shader, const std::string& type) {
    GLint infoLogLength;
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            std::vector<char> infoLog(infoLogLength);
            glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog.data());
            std::cout << "INFO_LOG::" << type << "_SHADER::\n" << infoLog.data() << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {
            std::vector<char> infoLog(infoLogLength);
            glGetProgramInfoLog(shader, infoLogLength, NULL, infoLog.data());
            std::cout << "INFO_LOG::PROGRAM::\n" << infoLog.data() << std::endl;
        }
    }
}

void checkGLError(const std::string& context) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error in " << context << ": " << err << std::endl;
    }
}

std::string readShaderCode(const std::string& shaderFilePath) {
    std::ifstream shaderFile;
    std::string shaderCode;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(shaderFilePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (const std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }
    return shaderCode;
}

unsigned int compileShader(const std::string& sourceCode, GLenum shaderType) {
    const char* shaderCode = sourceCode.c_str();
    unsigned int shader = glCreateShader(shaderType);
    checkGLError("glCreateShader");

    glShaderSource(shader, 1, &shaderCode, NULL);
    checkGLError("glShaderSource");

    glCompileShader(shader);
    checkGLError("glCompileShader");

    std::string shaderTypeName = shaderType == GL_VERTEX_SHADER ? "VERTEX" :
        shaderType == GL_FRAGMENT_SHADER ? "FRAGMENT" : "GEOMETRY";

    checkCompileErrors(shader, shaderTypeName);
    logShaderInfo(shader, shaderTypeName);

    return shader;
}

int LoadShaders(const std::string vertex_file_path, const std::string fragment_file_path, const std::string geometry_file_path) {
    // Read shader codes
    std::string vertexCode = readShaderCode(vertex_file_path);
    std::string fragmentCode = readShaderCode(fragment_file_path);
    //std::string geometryCode = readShaderCode(geometry_file_path);

    // Compile individual shaders
    unsigned int vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
    unsigned int fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);
    //unsigned int geometry = compileShader(geometryCode, GL_GEOMETRY_SHADER);

    // Create and link shader program
    GLuint programID = glCreateProgram();
    checkGLError("glCreateProgram");

    glAttachShader(programID, vertex);
    checkGLError("glAttachShader - vertex");

    glAttachShader(programID, fragment);
    checkGLError("glAttachShader - fragment");

    //glAttachShader(programID, geometry);
    checkGLError("glAttachShader - geometry");

    glLinkProgram(programID);
    checkGLError("glLinkProgram");

    checkCompileErrors(programID, "PROGRAM");
    logShaderInfo(programID, "PROGRAM");

    // Cleanup shaders
    glDeleteShader(vertex);
    checkGLError("glDeleteShader - vertex");

    glDeleteShader(fragment);
    checkGLError("glDeleteShader - fragment");

    //glDeleteShader(geometry);
    //checkGLError("glDeleteShader - geometry");

    return programID;
}
