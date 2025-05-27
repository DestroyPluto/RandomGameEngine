#include "SceneManager.h"
#include <SceneIO.h>

using namespace client;
using namespace core;
using namespace io;

HgError SceneManager::loadScene(std::string path){
    SceneIO loader = SceneIO();
    std::vector<Entity*> entities = loader.loadScene(path.c_str());
    
    if(entities.empty())
        return HgError::eFailure;

    Scene scene = Scene(entities, m_engine);

    m_scenes.emplace(path, std::move(scene));

    if(!m_currentScene)
        m_currentScene = &m_scenes[path];
    
    return HgError::eSuccess;
}

HgError SceneManager::saveScene(std::string path){
    SceneIO loader = SceneIO();
    std::vector<Entity*> ents = m_currentScene->getEntities();
    loader.saveScene(path.c_str(), ents);
    
    return HgError::eSuccess;
}

void SceneManager::changeScene(std::string path){
    if(m_scenes.contains(path)){
        m_currentScene = &m_scenes[path];
    }
}

SceneManager::SceneManager(){
    m_scenes = std::unordered_map<std::string, core::Scene>();
}

void SceneManager::setCameraPosition(glm::vec3 position){
    m_engine->setCameraPos(position);
}

glm::vec3 SceneManager::getCameraPosition(){
    return m_engine->getCameraPos();
}