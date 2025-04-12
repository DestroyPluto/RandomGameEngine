#pragma once
#include "HgError.h"
#include "Scene.h"
#include <unordered_map>
#include <string>
#include "RenderingPlugin.h"

namespace client{
    class SceneManager{
    public:
        static SceneManager* getInstance(){
            static SceneManager instance;
            return &instance;
        }

        core::HgError loadScene(std::string path, core::RenderingPlugin* engine);
        core::HgError saveScene(std::string path);
        
        //void setScene(std::string name);

        core::Scene* getCurrentScene(){
            if(m_currentScene)
                return m_currentScene;
            return nullptr;
        }

    private:
        SceneManager();
        std::unordered_map<std::string, core::Scene> m_scenes;
        core::Scene* m_currentScene;
    };
}