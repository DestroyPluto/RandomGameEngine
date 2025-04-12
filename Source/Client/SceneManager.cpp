#include "SceneManager.h"
#include "SceneLoader.h"

using namespace client;
using namespace core;
using namespace io;

HgError SceneManager::loadScene(std::string path, core::RenderingPlugin* engine){
    SceneLoader loader = SceneLoader();
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
    return HgError::eNotImplemented;
}

SceneManager::SceneManager(){
    m_scenes = std::unordered_map<std::string, core::Scene>();
}