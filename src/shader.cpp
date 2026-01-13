#include "../header/shader.h"
#include "../header/log.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : programID(0)
{
    std::string vertCode = ReadFile(vertexPath);
    std::string fragCode = ReadFile(fragmentPath);

    if (vertCode.empty() || fragCode.empty()) {
        //std::cerr << "Shader: one or more shader files are empty or failed to load\n";
		LOG_WARNING("Shader: one or more shader files are empty or failed to load");
        return;
    }

    GLuint vertShader = 0, fragShader = 0;
    if (!CompileShader(vertCode.c_str(), GL_VERTEX_SHADER, vertShader)) {
       // std::cerr << "Shader: vertex compile failed\n";
		LOG_WARNING("Shader: vertex compile failed");
        return;
    }
    if (!CompileShader(fragCode.c_str(), GL_FRAGMENT_SHADER, fragShader)) {
        //std::cerr << "Shader: fragment compile failed\n";
		LOG_WARNING("Shader: fragment compile failed");
        glDeleteShader(vertShader);
        return;
    }

    // Link program
    programID = glCreateProgram();
    glAttachShader(programID, vertShader);
    glAttachShader(programID, fragShader);
    glLinkProgram(programID);

    // Check link status
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLen = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);
        std::string infoLog(logLen, '\0');
        glGetProgramInfoLog(programID, logLen, nullptr, &infoLog[0]);
        //std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        LOG_ERROR("ERROR::SHADER::PROGRAM::LINK_FAILED\n" + infoLog);
        glDeleteProgram(programID);
        programID = 0;
    }

    // shaders can be deleted once linked
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

Shader::~Shader() {
    if (programID) glDeleteProgram(programID);
}

void Shader::Use() const {
    if (programID) glUseProgram(programID);
}

void Shader::SetUniformVec3(const char* name, float x, float y, float z) const {
    if (!programID) return;
    GLint loc = glGetUniformLocation(programID, name);
    if (loc != -1) glUniform3f(loc, x, y, z);
}

void Shader::SetUniformFloat(const char* name, float value) const {
    if (!programID) return;
    GLint loc = glGetUniformLocation(programID, name);
    if (loc != -1) glUniform1f(loc, value);
}

bool Shader::CompileShader(const char* source, GLenum shaderType, GLuint& outShader) const {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        std::string infoLog(logLen, '\0');
        glGetShaderInfoLog(shader, logLen, nullptr, &infoLog[0]);
        //std::cerr << "ERROR::SHADER::COMPILE_FAILED\n" << infoLog << std::endl;
        LOG_ERROR("ERROR::SHADER::COMPILE_FAILED\n" + infoLog);
        glDeleteShader(shader);
        return false;
    }

    outShader = shader;
    return true;
}

std::string Shader::ReadFile(const std::string& path) const {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        //std::cerr << "Shader::ReadFile: failed to open " << path << std::endl;
        LOG_ERROR("Shader::ReadFile: failed to open " + path);
        return {};
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}