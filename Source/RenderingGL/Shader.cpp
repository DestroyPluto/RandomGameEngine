#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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
        fShaderStream << vShaderFile.rdbuf();

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
    uint32_t vertex, fragment;
    int32_t success;
    char infoLog[512];
    
    //vertex shader 
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    //check for errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER: VERTEX: COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //fragment shader 
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    //check for errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER: FRAGMENT: COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //create shader program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER: PROGRAM: LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::bind(){
    glUseProgram(m_id);
}