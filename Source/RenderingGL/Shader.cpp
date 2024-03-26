#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace rendering;

Shader::Shader(const char* vertexPath, const char* fragmentPath){
    
    //read shaders
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    } catch(std::ifstream::failure e) {
        std::cout << "ERROR: SHADER: FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //compileShaders
    uint32_t vertexId;
    uint32_t fragmentId;
    int32_t success;
    char infoLog[512];
    
    //vertex shader 
    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vShaderCode, NULL);
    glCompileShader(vertexId);

    //check for errors
    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER: VERTEX: COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //fragment shader 
    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &fShaderCode, NULL);
    glCompileShader(fragmentId);

    //check for errors
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER: FRAGMENT: COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //create shader program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertexId);
    glAttachShader(m_id, fragmentId);
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER: PROGRAM: LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void Shader::bind(){
    glUseProgram(m_id);
}

template<>
void Shader::setUniform(Shader::eUniformName name, float data){
    glUniform1f(glGetUniformLocation(m_id, m_locations[name].c_str()), data);
}

template<>
void Shader::setUniform(Shader::eUniformName name, glm::vec3 data){
    glUniform3fv(glGetUniformLocation(m_id, m_locations[name].c_str()), 1, glm::value_ptr(data));
}

template<>
void Shader::setUniform(Shader::eUniformName name, glm::mat4 data){
    glUniformMatrix4fv(glGetUniformLocation(m_id, m_locations[name].c_str()), 1, GL_FALSE, glm::value_ptr(data));
}

template<>
void Shader::setUniform(Shader::eUniformName name, glm::mat3 data){
    glUniformMatrix3fv(glGetUniformLocation(m_id, m_locations[name].c_str()), 1, GL_FALSE, glm::value_ptr(data));
}