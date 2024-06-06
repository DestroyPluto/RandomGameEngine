#include "RenderingEngine.h"
#include "HgError.h"
#include "BasicShader.h"
#include "Shader.h"


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

    BasicShader* basicShader = new BasicShader();
    basicShader->bind();
    
    basicShader->setColour(0.5f,0.0f,0.0f);
    
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
        for(auto rc = m_renderCommands.begin(); rc != m_renderCommands.end(); rc++){
            rc->second.execute();
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
            continue; //not implemented yet 
            //there are two possible things that need updating: the geometry, or the coordinates (or both lol)
        }else{
            HgError err = createRenderCommand(ent->getId(), ent->getMesh());
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

//note: should this pass in ent instead of mesh? ent has position data...
HgError RenderingEngine::createRenderCommand(uint32_t id, Mesh* mesh){
    
    //create a render command
    RenderCommand rc = RenderCommand(id, mesh);
    
    //insert the rendercommand into the map
    m_renderCommands.emplace(id, std::move(rc));
    printf("Created Render Command! \n");

    return HgError::eSuccess;
}