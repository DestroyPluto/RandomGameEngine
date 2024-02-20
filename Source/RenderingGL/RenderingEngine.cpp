#include "RenderingEngine.h"
#include "HgError.h"

using namespace rendering;
using namespace core;

void framebuffer_size_callback(GLFWwindow*, int32_t width, int32_t height){
    glViewport(0,0,width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::thread RenderingEngine::startPlugin(){
    return std::thread(&RenderingEngine::initPlugin, this);
}

HgError RenderingEngine::initPlugin() {

    //init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(800, 600, "Mercury", NULL, NULL);

    if(m_window == NULL){
        glfwTerminate();
        return HgError::eFailure;
    }
        glfwMakeContextCurrent(m_window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        return HgError::eFailure;
    }

    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    renderloop();

    return HgError::eSuccess;
}

void RenderingEngine::renderloop(){
     while(!glfwWindowShouldClose(m_window)){
        //get input
        processInput(m_window);

        //render commands go here
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //update buffers
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

}

HgError RenderingEngine::setDirtyEntities(){
    return HgError::eNotImplemented;
}

HgError RenderingEngine::closePlugin(){
    glfwTerminate();
    return HgError::eNotImplemented;
}