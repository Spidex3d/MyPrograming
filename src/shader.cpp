#include "../header/shader.h"
#include "../header/log.h"

#include <fstream> // for file reading
#include <sstream> // for string stream
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : programID(0)
{
	std::string vertCode = ReadFile(vertexPath);  // read vertex shader code
	std::string fragCode = ReadFile(fragmentPath); // read fragment shader code

	if (vertCode.empty() || fragCode.empty()) { // do a basic check for empty shader code
		//std::cerr << "Shader: one or more shader files are empty or failed to load\n";
		LOG_WARNING("Shader: one or more shader files are empty or failed to load");
        return;
    }

    GLuint vertShader = 0, fragShader = 0;
    if (!CompileShader(vertCode.c_str(), GL_VERTEX_SHADER, vertShader)) { 
		// this will log the error if the code is incorrect
		LOG_WARNING("Shader: vertex compile failed");
        return;
    }
    if (!CompileShader(fragCode.c_str(), GL_FRAGMENT_SHADER, fragShader)) {
		// this will log the error if the code is incorrect
		LOG_WARNING("Shader: fragment compile failed");
		glDeleteShader(vertShader); // clean up vertex shader
        return;
    }

    // Link program
	programID = glCreateProgram();          // create shader program
	glAttachShader(programID, vertShader);  // attach vertex shader
	glAttachShader(programID, fragShader);  // attach fragment shader
	glLinkProgram(programID);               // link the program

    // Check link status
    GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success); // check for any linking errors
    if (!success) {
		GLint logLen = 0; // get length of info log
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen); // retrieve the length of the program info log
		std::string infoLog(logLen, '\0');                      // create a string to hold the log
		glGetProgramInfoLog(programID, logLen, nullptr, &infoLog[0]); // get the log
		LOG_ERROR("ERROR::SHADER::PROGRAM::LINK_FAILED\n" + infoLog); // show any linking errors
		glDeleteProgram(programID); // delete the program clean up
		programID = 0; // reset programID to 0 on failure
    }

	// shaders can be deleted once linked into the program
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

Shader::~Shader() {
	if (programID) glDeleteProgram(programID); // delete the shader program if it exists
}

void Shader::Use() const {
	if (programID) glUseProgram(programID); // use / bind the shader program
}
// set a vec3 uniform variable in the shader
void Shader::SetUniformVec3(const char* name, float x, float y, float z) const {
    if (!programID) return;
	GLint loc = glGetUniformLocation(programID, name); // get the location of the uniform variable
    if (loc != -1) glUniform3f(loc, x, y, z);
}
// set a float uniform variable in the shader
void Shader::SetUniformFloat(const char* name, float value) const {
    if (!programID) return;
	GLint loc = glGetUniformLocation(programID, name); // get the location of the uniform variable
    if (loc != -1) glUniform1f(loc, value);
}
// Compile a shader from source code
bool Shader::CompileShader(const char* source, GLenum shaderType, GLuint& outShader) const {
	GLuint shader = glCreateShader(shaderType); // create shader object
	glShaderSource(shader, 1, &source, nullptr);// set shader source code
	glCompileShader(shader);		  // compile the shader 

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
		GLint logLen = 0; // get length of info log
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen); // retrieve the length of the shader info log
		std::string infoLog(logLen, '\0'); // create a string to hold the log
		glGetShaderInfoLog(shader, logLen, nullptr, &infoLog[0]); // get the log
		LOG_ERROR("ERROR::SHADER::COMPILE_FAILED\n" + infoLog); // log the error message
		glDeleteShader(shader); // clean up shader
        return false;
    }

	outShader = shader; // set output shader if compilation was successful
    return true;
}

std::string Shader::ReadFile(const std::string& path) const {
	std::ifstream file(path, std::ios::in | std::ios::binary); // open file in binary mode
	if (!file) { // if file failed to open show an error
        LOG_ERROR("Shader::ReadFile: failed to open " + path);
        return {};
    }
	std::ostringstream ss;	//if the file opened successfully read its contents into a string stream
	ss << file.rdbuf();		// read file contents into stringstream
	return ss.str();		// return the file contents as a string
}