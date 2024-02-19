#include <stdio.h>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderingGL/RenderingEngine.h"
#include "Core/RenderingPlugin.h"


using namespace core;
using namespace rendering;

void framebuffer_size_callback(GLFWwindow*, int32_t width, int32_t height){
    glViewport(0,0,width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int args, char** argv){

    std::unique_ptr<RenderingEngine> engine = std::make_unique<RenderingEngine>();
    HgError err = engine->initPlugin();
    if(err == HgError::eSuccess){
        printf("It's Working!\n");
    }else{
        printf("Something is wrong :/\n");
        printf("Error code: %i\n", err);
    }

    //init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create a window
    GLFWwindow* window = glfwCreateWindow(800,600, "Mercury", NULL, NULL);

    if(window == NULL){
        printf("Failed to create GLFW Window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Failed to init GLAD\n");
        return -1;
    }

    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)){
        //get input
        processInput(window);

        //render commands go here
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //update buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    engine->closePlugin();
    engine.reset();
    glfwTerminate();
    return 0;
}