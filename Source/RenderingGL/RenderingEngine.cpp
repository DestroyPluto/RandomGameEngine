#include "RenderingEngine.h"
#include "HgError.h"
#include "BasicShader.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "glad/glad.h"
#include <HgLogger.h>
#include "DisplayText.h"
#include "config.h"


using namespace rendering;
using namespace core;

void framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height){
    glViewport(0,0, width, height);
    RenderingEngine* renderingEngine = static_cast<RenderingEngine*>(glfwGetWindowUserPointer(window));
    Camera* camera = renderingEngine->getCamera();
    assert(camera);
    camera->setScreenWidth((float)width);
    camera->setScreenHeight((float)height);
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

void mouse_callback(GLFWwindow* window, double xPos, double yPos){
    RenderingEngine* renderingEngine = static_cast<RenderingEngine*>(glfwGetWindowUserPointer(window));
    assert(renderingEngine->m_mouseCallback); //if no key callback is set, then we should fail

    int windowX;
    int windowY;

    glfwGetWindowSize(window, &windowX, &windowY);
    //x and y to clip coords
    float x = 2 * ((float)xPos/(float)windowX) - 1;
    float y = 2 * ((float)yPos/(float)windowY) - 1;
    y = -y;
    renderingEngine->m_mouseCallback(x, y);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    RenderingEngine* renderingEngine = static_cast<RenderingEngine*>(glfwGetWindowUserPointer(window));
    assert(renderingEngine->m_mouseButtonCallback); //if no key callback is set, then we should fail
    renderingEngine->m_mouseButtonCallback(button, action);
}

RenderingEngine::RenderingEngine(std::shared_ptr<core::Config> config){
    m_config = config;
    //these cannot be initialised until after opengl has been initialised
    m_window = nullptr;
    m_camera = nullptr;
    m_basicShader = nullptr;
    m_textShader = nullptr;
}

HgError RenderingEngine::initPlugin() {
    //init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_camera = new Camera();
    m_camera->setScreenWidth(800.0f);
    m_camera->setScreenHeight(600.0f);
    m_window = glfwCreateWindow(800.0f, 600.0f, "Mercury", NULL, NULL);

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
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    m_basicShader = new BasicShader(m_config);
    m_basicShader->bind();
    
    m_basicShader->setColour(0.5f,0.0f,0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    m_basicShader->setModelMatrix(model);

    m_textShader = new TextShader(m_config);
    m_textShader->bind();
    m_textShader->setModelMatrix(model);
    m_textShader->unBind();

    m_isInitialized = true;
    renderloop();

    return HgError::eSuccess;
}

void RenderingEngine::renderloop(){

     while(!glfwWindowShouldClose(m_window)){
        glfwPollEvents();
        //get input
        processInput(m_window);

        //render commands go here
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //geometry pass - should probably move this into it's own thing.
        //should also have a different pass for each shader type.
        m_basicShader->bind();
        for(auto rc = m_renderCommands.begin(); rc != m_renderCommands.end(); rc++){
            if(rc->second.getKey() < 1){
                //m_basicShader->setProjectionMatrix(m_camera->getOrtho());
                m_basicShader->setProjectionMatrix(glm::mat4(1.0));
                m_basicShader->setViewMatrix(glm::mat4(1.0));
            }else{
                m_basicShader->setProjectionMatrix(m_camera->getPerspective());
                m_basicShader->setViewMatrix(m_camera->getView());
            }
            rc->second.execute(m_basicShader);
        }
        m_basicShader->unBind();

        m_textShader->bind();
        for(auto rc = m_TextRenderCommands.begin(); rc != m_TextRenderCommands.end(); rc++){
            m_textShader->setProjectionMatrix(m_camera->getOrtho());
            m_textShader->setViewMatrix(glm::mat4(1.0));

            rc->second.execute();
        }
        m_textShader->unBind();

        //update buffers
        glfwSwapBuffers(m_window);

        //update entities for the next frame
        handleDirtyEnts();

        //load any textures
        handleDirtyTextures();

    }
    //make sure to tell the main thread we are exiting
    m_keyCallback(GLFW_KEY_ESCAPE, GLFW_PRESS);
}

HgError RenderingEngine::setDirtyEntities(std::vector<Entity*>& entities){
    std::lock_guard<std::mutex>lock(m_RenderingMutex);

    for(Entity* ent : entities){
        m_dirtyEntities.insert(ent);
    }
    return HgError::eSuccess;
}

HgError RenderingEngine::addTexture(HgTexture* texture) {
    std::lock_guard<std::mutex>lock(m_RenderingMutex);
        m_dirtyTextures.insert(texture);
    return HgError::eSuccess;
}

HgError RenderingEngine::closePlugin(){
    glfwTerminate();
    return HgError::eSuccess;
}

HgError RenderingEngine::setCameraPos(glm::vec3 position){
    std::lock_guard<std::mutex>lock(m_RenderingMutex);
    if(m_camera){
        m_camera->setPosition(position);
        return HgError::eSuccess;
    }
    return HgError::eFailure;
}

glm::vec3 RenderingEngine::getCameraPos(){
    return m_camera->getPosition();
}

void RenderingEngine::handleDirtyEnts(){
    std::lock_guard<std::mutex>lock(m_RenderingMutex);
    //if we don't have anything to update, return early   
    if(m_dirtyEntities.empty())
        return;
    for(Entity* ent : m_dirtyEntities){
        //check if we already have render commands for entity
        if(m_renderCommands.contains(ent->getId()) || m_TextRenderCommands.contains(ent->getId())){
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


void RenderingEngine::handleDirtyTextures(){
    std::lock_guard<std::mutex>lock(m_RenderingMutex);
    if(m_dirtyTextures.empty())
        return;

    unsigned int id;
        
    for(HgTexture* tex : m_dirtyTextures){
        //if there is already a texture, then it has already been loaded.
        if(tex->getId() == 0){        
            glGenTextures(1, &id);
            tex->setId(id);
        }else{
            id = tex->getId();
        }

        glBindTexture(GL_TEXTURE_2D, id);
        //TODO: configure num channels correctly - currently assume RGBA, but that might not neccessarialy be the case.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->getWidth(), tex->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->getData());
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        //either update or insert into our map.
        const char* key = tex->getPath();
        m_textures[key] = tex;    
    }

    m_dirtyEntities.clear();
}

HgError RenderingEngine::updateRenderCommand(Entity* ent){
    DisplayText* txt = dynamic_cast<DisplayText*>(ent);
    if(txt){        
        auto textIndex = m_TextRenderCommands.find(ent->getId());
        if(textIndex != m_TextRenderCommands.end()){
            TextRenderCommand* rc = &(textIndex->second);
            rc->setPosition(txt->getPosition());
            rc->setText(txt->getText());
            return HgError::eSuccess; 
        }
        return HgError::eFailure;
    }

    auto index = m_renderCommands.find(ent->getId());
    if(index != m_renderCommands.end()){
        RenderCommand* rc = &(index->second);
        rc->updateModelMatrix(ent->getPosition(), ent->getRotation(), ent->getScale());
        rc->setTextureID(ent->getTextureId());
        return HgError::eSuccess;
    }

    return HgError::eFailure;
}

HgError RenderingEngine::createRenderCommand(Entity* ent){
    //TODO: proper key generation.
    uint32_t key = ent->getLayer();

    //text entities have a different render command
    if(ent->getLayer() == core::eText){
        DisplayText* txt = dynamic_cast<DisplayText*>(ent);
        if(txt){
            TextRenderCommand rc = TextRenderCommand(txt->getId(), txt->getText());
            rc.setPosition(txt->getPosition());
            m_TextRenderCommands.emplace(txt->getId(), std::move(rc));
        }
    }else{
        //create a render command
        RenderCommand rc = RenderCommand(ent->getId(), ent->getMesh(), ent->getTextureId(), key);
        rc.updateModelMatrix(ent->getPosition(), ent->getRotation(), ent->getScale());
        //insert the rendercommand into the map
        m_renderCommands.emplace(ent->getId(), std::move(rc));
        HgLogger::logMsg("Created Render Command!");
    } 
    return HgError::eSuccess;
}