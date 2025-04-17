#include "SceneManager.h"
#include "SceneIO.h"

using namespace client;
using namespace core;
using namespace io;

HgError SceneManager::loadScene(std::string path, core::RenderingPlugin* engine){
    SceneIO loader = SceneIO();
    std::vector<Entity*> entities = loader.loadScene(path.c_str());
    
    if(entities.empty())
        return HgError::eFailure;

    Scene scene = Scene(entities, engine);

    m_scenes.emplace(path, std::move(scene));

    if(!m_currentScene)
        m_currentScene = &m_scenes[path];
    
    return HgError::eSuccess;
}

HgError SceneManager::saveScene(std::string path){
    SceneIO loader = SceneIO();
    loader.saveScene(path.c_str(), m_currentScene->getEntities());
    
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