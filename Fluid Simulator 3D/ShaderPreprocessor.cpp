#include "ShaderPreprocessor.h"

std::string ShaderPreprocessor::readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string ShaderPreprocessor::preprocessShader(const std::string& filePath) {
    return preprocessShader(filePath, cache);
}

std::string ShaderPreprocessor::preprocessShader(const std::string& filePath, std::unordered_map<std::string, std::string>& cache) {
    if (cache.find(filePath) != cache.end()) {
        return cache[filePath];
    }

    std::string shaderCode = readFile(filePath);
    std::regex includePattern(R"(#include\s*\"(.+?)\")");
    std::smatch matches;

    while (std::regex_search(shaderCode, matches, includePattern)) {
        std::string includeFilePath = matches[1].str();
        std::string includeFileContent = preprocessShader(includeFilePath, cache);
        shaderCode.replace(matches.position(0), matches.length(0), includeFileContent);
    }

    cache[filePath] = shaderCode;
    return shaderCode;
}
