#ifndef SHADER_PREPROCESSOR_H
#define SHADER_PREPROCESSOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <unordered_map>

class ShaderPreprocessor {
public:
    ShaderPreprocessor() = default;

    // Read and preprocess shader file
    std::string preprocessShader(const std::string& filePath);

private:
    std::unordered_map<std::string, std::string> cache;

    // Recursive function to preprocess shader includes
    std::string preprocessShader(const std::string& filePath, std::unordered_map<std::string, std::string>& cache);
    std::string readFile(const std::string& filePath);
};

#endif // SHADER_PREPROCESSOR_H
