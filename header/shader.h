#pragma once
#include <string>
#include <glad/glad.h>

class Shader {
public:
    // Construct and build the shader from file paths
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    // Use / bind the shader program
    void Use() const;

    // convenience uniform setters
    void SetUniformVec3(const char* name, float x, float y, float z) const;
    void SetUniformFloat(const char* name, float value) const;
    void SetUniformInt(const char* name, int value) const;

    
    GLuint ID() const { return programID; }

private:
	// private member can only be accessed by member functions and not outside the class
    GLuint programID;
	// Helper functions for shader compilation and file reading
    bool CompileShader(const char* source, GLenum shaderType, GLuint& outShader) const;
	// Read shader source code from file
    std::string ReadFile(const std::string& path) const;
};
