#include "RenderingEngine.h"
#include "HgError.h"
#include "BasicShader.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    RenderingEngine* renderingEngine = static_cast<RenderingEngine*>(glfwGetWindowUserPointer(window));
    assert(renderingEngine->m_keyCallback); //if no key callback is set, then we should fail
    renderingEngine->m_keyCallback(key, action); 
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
    glfwSetWindowUserPointer(m_window, this);
    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetKeyCallback(m_window, key_callback);

    m_basicShader = new BasicShader();
    m_basicShader->bind();
    
    m_basicShader->setColour(0.5f,0.0f,0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-7.0f));
    m_basicShader->setViewMatrix(view);
    m_basicShader->setModelMatrix(model);
    m_basicShader->setProjectionMatrix(glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 500.0f));
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

        //geometry pass - should probably move this into it's own thing.
        //should also have a different pass for each shader type.
        for(auto rc = m_renderCommands.begin(); rc != m_renderCommands.end(); rc++){
            rc->second.execute(m_basicShader);
        }

        //update buffers
        glfwSwapBuffers(m_window);
        glfwPollEvents();

        //update entities for the next frame
        handleDirtyEnts();
    }
}

HgError RenderingEngine::setDirtyEntities(std::vector<Entity*>& entities){
    std::lock_guard<std::mutex>lock(m_RenderingMutex);
    m_dirtyEntities.insert(m_dirtyEntities.end(), entities.begin(), entities.end());
    return HgError::eSuccess;
}

HgError RenderingEngine::closePlugin(){
    glfwTerminate();
    return HgError::eSuccess;
}

void RenderingEngine::handleDirtyEnts(){
    std::lock_guard<std::mutex>lock(m_RenderingMutex);
    //if we don't have anything to update, return early   
    if(m_dirtyEntities.empty())
        return;
    for(Entity* ent : m_dirtyEntities){
        //check if we already have render commands for entity
        if(m_renderCommands.contains(ent->getId())){
            //there are two possible things that need updating: the geometry, or the coordinates (or both lol)
            //currently only updates the coords
            updateRenderCommand(ent);
        }else{
            HgError err = createRenderCommand(ent);
            //if it fails, try again next time lol
            if(err != HgError::eSuccess)
                continue;
        }

        //reached the end, assume success, therefore it is no longer dirty
        ent->setDirty(false);
    }

    //clear dirty ents
    m_dirtyEntities.clear();
}

HgError RenderingEngine::updateRenderCommand(Entity* ent){
    auto index = m_renderCommands.find(ent->getId());
    if(index != m_renderCommands.end()){
        RenderCommand* rc = &(index->second);
        rc->updateModelMatrix(ent->getPosition(), ent->getRotation(), ent->getScale());
        return HgError::eSuccess;
    }
    return HgError::eFailure;
}

//note: should this pass in ent instead of mesh? ent has position data...
HgError RenderingEngine::createRenderCommand(Entity* ent){
    //create a render command
    RenderCommand rc = RenderCommand(ent->getId(), ent->getMesh());
    rc.updateModelMatrix(ent->getPosition(), ent->getRotation(), ent->getScale());
    //insert the rendercommand into the map
    m_renderCommands.emplace(ent->getId(), std::move(rc));
    printf("Created Render Command! \n");

    return HgError::eSuccess;
}